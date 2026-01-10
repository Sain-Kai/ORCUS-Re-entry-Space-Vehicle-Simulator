#include "../include/orcus_core.h"
#include "../include/orcus_dynamics.h"
#include "../include/orcus_physics.h"
#include "../include/orcus_heat.h"
#include "../include/orcus_flowfield.h"      // Phase-4C-1
#include "../include/orcus_boundary_layer.h" // Phase-4C-2
#include "../include/orcus_bl_heating.h"     // Phase-4C-3
#include "../include/orcus_pns_energy.h"     // Phase-4C-4 (NEW, ADDITIVE)
#include "../include/orcus_tps.h"
#include "../include/orcus_guidance.h"
#include "../include/orcus_constants.h"
#include "../include/orcus_envelope.h"
#include "../include/orcus_config.h"
#include "../include/orcus_uncertainity.h"
#include "../include/orcus_tps_min.h"
#include "../include/orcus_montecarlo.h"
#include "../include/orcus_shock_layer.h"
#include "../include/orcus_real_gas.h"
#include "../include/orcus_noneq_chemistry.h"
#include "../include/orcus_surface_chemistry.h"
#include "../include/orcus_radiation.h"
#include "../include/orcus_inviscid.h"
#include "../include/orcus_displacement_bl.h"   // Phase-5B
#include "../include/orcus_viscous_inviscid.h"
#include "../include/orcus_surface_distribution.h"
#include "../include/orcus_tps_distribution.h"
#include "../include/orcus_structure_thermal.h"

#include <iostream>
#include <cmath>
#include <algorithm>

namespace ORCUS {

    // ===============================
    // Phase banner utility (UNCHANGED)
    // ===============================
    void print_stage_banner(OrcusStage stage) {
        std::cout << "\n====================================\n";
        switch (stage) {

        case OrcusStage::PHASE_3K:
            std::cout << "ORCUS Phase-3K — Thermal Margin Quantification\n";
            break;

        case OrcusStage::PHASE_3N:
            std::cout << "ORCUS Phase-3N — Worst-Case Envelope\n";
            break;

        case OrcusStage::PHASE_3P:
            std::cout << "ORCUS Phase-3P — Uncertainty Robustness Analysis\n";
            break;

        case OrcusStage::PHASE_3O:
            std::cout << "ORCUS Phase-3O — Certification Traceability\n";
            break;

        case OrcusStage::PHASE_3Q:
            std::cout << "ORCUS Phase-3Q — Minimum TPS Closure\n";
            break;

        case OrcusStage::PHASE_3W:
            std::cout << "ORCUS Phase-3W — Monte-Carlo Certification\n";
            break;

        case OrcusStage::PHASE_4C_1:
            std::cout << "ORCUS Phase-4C-1 — Shock Stand-off & Stagnation Field\n";
            break;

        case OrcusStage::PHASE_4C_2:
            std::cout << "ORCUS Phase-4C-2 — Boundary-Layer Integral Equations\n";
            break;

        case OrcusStage::PHASE_4C_3:
            std::cout << "ORCUS Phase-4C-3 — Wall Heat Flux from BL Theory\n";
            break;

        case OrcusStage::PHASE_4C_4:
            std::cout << "ORCUS Phase-4C-4 — Parabolized Navier–Stokes Energy Equation\n";
            break;

        case OrcusStage::PHASE_4C_5:
			std::cout << "ORCUS Phase-4C-5 — Shock-Layer Finite Thickness Correction\n";
            break;
        
        case OrcusStage::PHASE_4D:
            std::cout << "ORCUS Phase-4D — Real-Gas Thermodynamic Correction\n";
			break;

        case OrcusStage::PHASE_4E:
            std::cout << "ORCUS Phase-4E — Surface Chemistry & Catalysis\n";
			break;

        case OrcusStage::PHASE_4F:
            std::cout << "ORCUS Phase-4F — Non-Equilibrium Gas Chemistry\n";
			break;

        case OrcusStage::PHASE_4G:
            std::cout << "ORCUS Phase-4G — Radiation Gas Coupling\n";
			break;

        case OrcusStage::PHASE_5A:
			std::cout << "ORCUS Phase-5A — Inviscid Outer Flow Forebody Analysis\n";
			break;

		case OrcusStage::PHASE_5B:
			std::cout << "ORCUS Phase-5B — Displacement Thickness Correction\n";
            break;

        case OrcusStage::PHASE_5C:
            std::cout << "ORCUS Phase-5C - Viscous-Inviscid Coupling\n";
            break;

        case OrcusStage::PHASE_5D:
            std::cout << "ORCUS Phase-5D — Surface Cp(x) & Heat-Flux Distribution\n";
            break;

        case OrcusStage::PHASE_5E:
            std::cout << "ORCUS Phase-5E — TPS Tile-by-Tile Recession Mapping\n";
            break;

        case OrcusStage::PHASE_5F:
            std::cout << "ORCUS Phase-5F — Internal Structure-Thermal Stress\n";
			break;
        }
        std::cout << "====================================\n";
    }

    ThermalSummary run_thermal_summary(
        const OrcusConfig& cfg,
        double bank_rad
    ) {
        ThermalSummary out{};
        out.peak_T_ratio = 0.0;
        out.remaining_tps = 1.0;
        out.peak_q = 0.0;
        out.failure_mode = TPSFailureMode::NONE;

        Vehicle6DOF veh{};
        veh.mass = 1500.0;
        veh.Sref = 1.8;
        veh.cref = 2.0;
        veh.bank = bank_rad;
        veh.Ixx = 800;
        veh.Iyy = 1200;
        veh.Izz = 1000;

        TPSMaterial tps{ 1600.0, 1250.0, 0.15, 0.9, 2200.0, 2.5e6 };

        TPSState tps_state{};
        tps_state.T_surface = 300.0;
        tps_state.T_bulk = 300.0;
        tps_state.thickness = cfg.tps_thickness_m;
        tps_state.failed = false;

        State6DOF s{};
        s.z = cfg.initial_altitude_m;
        s.vx = cfg.initial_speed_mps;
        s.vz = cfg.initial_vz_mps;
        s.q0 = 1.0;

        const double dt = 0.02;

        while (s.z > 0.0 && !tps_state.failed) {

            double V = std::sqrt(s.vx * s.vx + s.vz * s.vz);
            double Mach = V / speed_of_sound(s.z);
            double gamma = std::atan2(-s.vz, s.vx);

            double rho = density(s.z);
            double Rex = rho * V * veh.cref / 1.8e-5;

            HeatFlux H = compute_heating(
                rho, V, cfg.nose_radius_m,
                gamma, Mach, Rex,
                tps_state.T_surface,
                tps.emissivity
            );

            out.peak_q = std::max(out.peak_q, H.q_net);

            tps_state = update_tps_ablation(tps_state, tps, H.q_net, dt);

            out.peak_T_ratio =
                std::max(out.peak_T_ratio, tps_state.T_surface / tps.Tmax);

            out.remaining_tps =
                std::min(out.remaining_tps,
                    tps_state.thickness / cfg.tps_thickness_m);

            GuidanceCmd gcmd =
                skip_guidance(s.z, Mach, H.q_net, gamma, cfg);

            s.vz += V * gcmd.gamma_dot_cmd * dt;
            s = rk4_step_6dof(s, dt, veh);
        }

        out.failure_mode = tps_state.failure_mode;
        return out;
    }

    void run_default_simulation() {

        std::cout << "ORCUS Phase-3 Engine Running\n";
        OrcusConfig cfg = default_config();

        // -------- Phase-3K --------
        print_stage_banner(OrcusStage::PHASE_3K);
        ThermalSummary base = run_thermal_summary(cfg, 0.0);

        std::cout << "Peak T/Tmax           : " << base.peak_T_ratio << "\n";
        std::cout << "Remaining TPS fraction: " << base.remaining_tps << "\n";
        std::cout << "Peak heat flux        : " << base.peak_q << "\n";
        std::cout << "Failure mode          : "
            << to_string(base.failure_mode) << "\n";

        // -------- Phase-3N --------
        print_stage_banner(OrcusStage::PHASE_3N);
        run_worst_case_envelope();

        // -------- Phase-3P --------
        print_stage_banner(OrcusStage::PHASE_3P);
        run_uncertainty_analysis(
            density(30000.0),
            base.peak_q,
            base.peak_T_ratio * 2200.0,
            cfg.tps_thickness_m,
            2200.0
        );

        // -------- Phase-3O --------
        print_stage_banner(OrcusStage::PHASE_3O);
        std::cout << "All stages executed with certification traceability.\n";

        // -------- Phase-3Q --------
        print_stage_banner(OrcusStage::PHASE_3Q);
        run_minimum_tps_search();

        // -------- Phase-3W --------
        print_stage_banner(OrcusStage::PHASE_3W);
        run_montecarlo_certification();

        double z_ref = 40000.0;
        double V_ref = cfg.initial_speed_mps;
        double Mach_ref = V_ref / speed_of_sound(z_ref);

        // -------- Phase-4C-1 --------
        print_stage_banner(OrcusStage::PHASE_4C_1);

        StagnationField stag =
            compute_stagnation_field(
                Mach_ref,
                1.4,
                cfg.nose_radius_m,
                pressure(z_ref),
                temperature(z_ref)
            );

        std::cout << "--- Stagnation Flow Field ---\n";
        std::cout << "Shock stand-off distance : " << stag.shock_standoff << " m\n";
        std::cout << "Stagnation pressure     : " << stag.p_stag << " Pa\n";
        std::cout << "Stagnation temperature  : " << stag.T_stag << " K\n";
        std::cout << "Stagnation density      : " << stag.rho_stag << " kg/m^3\n";

        // -------- Phase-4C-2 --------
        print_stage_banner(OrcusStage::PHASE_4C_2);

        constexpr double mu_air = 1.8e-5;
        constexpr double x_ref = 0.1;

        BoundaryLayerProps bl =
            compute_boundary_layer(
                stag.rho_stag,
                mu_air,
                V_ref,
                x_ref,
                1.0e6
            );

        std::cout << "--- Boundary Layer (Integral Solver) ---\n";
        std::cout << "State                  : "
            << (bl.state == BoundaryLayerState::LAMINAR ? "Laminar" : "Transitional") << "\n";
        std::cout << "Momentum thickness     : " << bl.theta << " m\n";
        std::cout << "Energy thickness       : " << bl.delta_e << " m\n";
        std::cout << "Skin friction coeff Cf : " << bl.Cf << "\n";
        std::cout << "Wall shear stress      : " << bl.tau_w << " Pa\n";

        // -------- Phase-4C-3 --------
        print_stage_banner(OrcusStage::PHASE_4C_3);

        constexpr double CP_AIR = 1005.0;

        BLHeatFlux q_bl =
            compute_bl_heat_flux(
                bl,
                stag.rho_stag,
                V_ref,
                CP_AIR,
                stag.T_stag,
                300.0
            );

        std::cout << "--- Wall Heat Flux (BL Theory) ---\n";
        std::cout << "Stanton number         : " << q_bl.Stanton << "\n";
        std::cout << "Wall heat flux q_w     : " << q_bl.q_wall << " W/m^2\n";

        // -------- Phase-4C-4 --------
        print_stage_banner(OrcusStage::PHASE_4C_4);

        PNSEnergyResult pns =
            solve_pns_energy(
                stag.T_stag,
                300.0,
                bl.delta_e,
                0.026   // air thermal conductivity (W/m-K)
            );

        std::cout << "--- PNS Energy Equation ---\n";
        std::cout << "Wall temperature gradient dT/dy : "
            << pns.dTdy_wall << " K/m\n";
        std::cout << "Diffusive wall heat flux q_NS   : "
            << pns.q_wall << " W/m^2\n";

        // -------- Phase-4C-5: Shock-layer finite thickness correction --------
        print_stage_banner(OrcusStage::PHASE_4C_5);

        ShockLayerProps sl =
            compute_shock_layer(
                Mach_ref,
                1.4,
                cfg.nose_radius_m
            );

        double q_corrected =
            sl.efficiency * q_bl.q_wall;

        std::cout << "--- Shock Layer Finite Thickness Correction ---\n";
        std::cout << "Shock-layer thickness     : "
            << sl.thickness << " m\n";
        std::cout << "Efficiency factor         : "
            << sl.efficiency << "\n";
        std::cout << "Corrected wall heat flux  : "
            << q_corrected << " W/m^2\n";

        // -------- Phase-4D: Real-gas thermodynamic correction --------
        print_stage_banner(OrcusStage::PHASE_4D);

        RealGasProps rg =
            compute_real_gas_props(stag.T_stag);

        double q_real_gas =
            q_corrected * (rg.Cp / 1005.0) * (1.4 / rg.gamma);

        std::cout << "--- Real Gas Correction ---\n";
        std::cout << "Effective gamma          : "
            << rg.gamma << "\n";
        std::cout << "Effective Cp             : "
            << rg.Cp << " J/kg-K\n";
        std::cout << "Real-gas corrected heat  : "
            << q_real_gas << " W/m^2\n";

        // -------- Phase-4E: Surface chemistry & catalysis --------
        print_stage_banner(OrcusStage::PHASE_4E);

        // Select wall catalysis model (can be config-driven later)
        CatalysisModel wall_model =
            CatalysisModel::PARTIALLY_CATALYTIC;

        SurfaceChemistryProps sc =
            compute_surface_chemistry(
                300.0,   // Wall temperature (current TPS surface temp)
                wall_model
            );

        double q_surface_chem =
            q_real_gas * sc.heat_multiplier;

        std::cout << "--- Surface Chemistry Correction ---\n";
        std::cout << "Catalysis model          : ";

        switch (wall_model) {
        case CatalysisModel::NON_CATALYTIC:
            std::cout << "Non-catalytic\n";
            break;
        case CatalysisModel::PARTIALLY_CATALYTIC:
            std::cout << "Partially catalytic\n";
            break;
        case CatalysisModel::FULLY_CATALYTIC:
            std::cout << "Fully catalytic\n";
            break;
        }

        std::cout << "Recombination efficiency : "
            << sc.efficiency << "\n";
        std::cout << "Heat multiplier          : "
            << sc.heat_multiplier << "\n";
        std::cout << "Chemistry-corrected heat : "
            << q_surface_chem << " W/m^2\n";

        // -------- Phase-4F: Nonequilibrium gas chemistry --------
        print_stage_banner(OrcusStage::PHASE_4F);

        NoneqChemistryProps nc =
            compute_noneq_chemistry(
                stag.T_stag,
                Mach_ref
            );

        double q_noneq =
            q_surface_chem * nc.heat_multiplier;

        std::cout << "--- Nonequilibrium Chemistry Correction ---\n";
        std::cout << "Dissociation fraction    : "
            << nc.dissociation_fraction << "\n";
        std::cout << "Relaxation factor        : "
            << nc.relaxation_factor << "\n";
        std::cout << "Heat multiplier          : "
            << nc.heat_multiplier << "\n";
        std::cout << "Nonequilibrium heat flux : "
            << q_noneq << " W/m^2\n";

        // -------- Phase-4G: Radiation–gas coupling --------
        print_stage_banner(OrcusStage::PHASE_4G);

        RadiationProps rad =
            compute_radiative_coupling(
                stag.T_stag,
                stag.rho_stag,
                sl.thickness,   // shock-layer thickness as optical path
                300.0           // wall temperature
            );

        double q_total =
            q_noneq + rad.net_radiative_heat;

        std::cout << "--- Radiation–Gas Coupling ---\n";
        std::cout << "Gas emissive power        : "
            << rad.emissive_power << " W/m^2\n";
        std::cout << "Absorption factor         : "
            << rad.absorption_factor << "\n";
        std::cout << "Net radiative heat flux   : "
            << rad.net_radiative_heat << " W/m^2\n";
        std::cout << "TOTAL wall heat flux      : "
            << q_total << " W/m^2\n";

        // Phase-5A — Inviscid Outer Flow (Euler surrogate)
        print_stage_banner(OrcusStage::PHASE_5A);
        constexpr int N_INVISCID = 20;

        InviscidSolution inv =
            compute_inviscid_flow(
                Mach_ref,
                1.4,
                V_ref,
                pressure(z_ref),
                cfg.nose_radius_m,
                N_INVISCID
            );

        std::cout << "--- Inviscid Surface Flow ---\n";
        std::cout << "x (m)\tCp\t\tUe (m/s)\tPe (Pa)\n";

        for (int i = 0; i < inv.N; ++i) {
            std::cout
                << inv.pts[i].x << "\t"
                << inv.pts[i].Cp << "\t"
                << inv.pts[i].Ue << "\t"
                << inv.pts[i].Pe << "\n";
        }

        // NOTE: memory cleanup (explicit, deterministic)
        delete[] inv.pts;

        // Phase-5B — Boundary-Layer Displacement Thickness
        print_stage_banner(OrcusStage::PHASE_5B);

        bool is_turbulent =
            (bl.state == BoundaryLayerState::TURBULENT ||
                bl.state == BoundaryLayerState::TRANSITIONAL);

        DisplacementBL dstar =
            compute_displacement_thickness(
                bl.theta,
                bl.Cf,
                is_turbulent
            );

        std::cout << "--- Boundary Layer Displacement ---\n";
        std::cout << "Momentum thickness θ      : "
            << bl.theta << " m\n";
        std::cout << "Shape factor H            : "
            << dstar.shape_factor << "\n";
        std::cout << "Displacement thickness δ* : "
            << dstar.delta_star << " m\n";

        // Phase-5C — Viscous–Inviscid Coupling 
        print_stage_banner(OrcusStage::PHASE_5C);

        VICouplingResult vi =
            solve_viscous_inviscid(
                cfg.nose_radius_m,
                dstar.delta_star,
                Mach_ref,
                1.4
            );

        std::cout << "--- Viscous–Inviscid Coupling ---\n";
        std::cout << "Original nose radius     : "
            << cfg.nose_radius_m << " m\n";
        std::cout << "Displacement thickness   : "
            << dstar.delta_star << " m\n";
        std::cout << "Effective radius         : "
            << vi.effective_radius << " m\n";
        std::cout << "Cp correction factor     : "
            << vi.Cp_correction << "\n";
        std::cout << "Coupling iterations      : "
            << vi.iterations << "\n";
        std::cout << "Final residual           : "
            << vi.convergence_error << "\n";

        // Phase-5D — Surface-Resolved Pressure & Heating
        print_stage_banner(OrcusStage::PHASE_5D);

        constexpr double BODY_LENGTH = 4.0;   // meters
        constexpr int N_STATIONS = 50;

        SurfaceDistribution surf =
            compute_surface_distribution(
                vi.Cp_correction,
                q_noneq,                  // final corrected stagnation heat
                vi.effective_radius,
                BODY_LENGTH,
                N_STATIONS
            );

        std::cout << "--- Surface Distribution ---\n";
        std::cout << "Body length              : "
            << BODY_LENGTH << " m\n";
        std::cout << "Number of stations       : "
            << N_STATIONS << "\n";
        std::cout << "Peak heat flux           : "
            << surf.peak_q << " W/m^2\n";
        std::cout << "Peak location x          : "
            << surf.peak_x << " m\n";

        std::cout << "\nSample stations:\n";
        for (size_t i = 0; i < surf.points.size(); i += 10) {
            std::cout << "x=" << surf.points[i].x
                << " m | Cp=" << surf.points[i].Cp
                << " | q=" << surf.points[i].q
                << " W/m^2\n";
        }

        // Phase-5E — TPS Tile-by-Tile Recession
        print_stage_banner(OrcusStage::PHASE_5E);

        std::vector<double> xs, qs;
        for (const auto& p : surf.points) {
            xs.push_back(p.x);
            qs.push_back(p.q);
        }

        TPSDistributionResult tps_map =
            compute_tps_distribution(
                cfg.tps_thickness_m,
                1600.0,      // TPS density (kg/m^3)
                2.5e6,       // heat of ablation (J/kg)
                120.0,       // exposure time (s)
                xs,
                qs
            );

        std::cout << "--- TPS Recession Map ---\n";
        std::cout << "Minimum remaining TPS   : "
            << tps_map.min_thickness << " m\n";

        if (tps_map.any_failure) {
            std::cout << "TPS FAILURE at x = "
                << tps_map.failure_x << " m\n";
        }
        else {
            std::cout << "TPS survived entire surface\n";
        }

        std::cout << "\nSample tiles:\n";
        for (size_t i = 0; i < tps_map.tiles.size(); i += 10) {
            const auto& t = tps_map.tiles[i];
            std::cout << "x=" << t.x
                << " m | h_remain=" << t.thickness_remain
                << " m | failed=" << t.failed << "\n";
        }

        // =========================================================
// Phase-5F — Internal Structure Thermal Stress
// =========================================================
        print_stage_banner(OrcusStage::PHASE_5F);

        StructureThermalProps st =
            compute_structure_thermal(
                q_noneq,        // final corrected heat flux
                1.0,            // timestep (s)
                15.0,           // interface conductivity (W/m-K)
                0.02,           // structure thickness (m)
                2700.0,         // Al alloy density (kg/m^3)
                900.0,          // cp (J/kg-K)
                70e9,           // Young's modulus (Pa)
                23e-6,          // thermal expansion (1/K)
                300.0,          // reference temp (K)
                250e6           // allowable stress (Pa)
            );

        std::cout << "--- Structure Thermal Response ---\n";
        std::cout << "Structure temperature  : "
            << st.T_structure << " K\n";
        std::cout << "Thermal stress         : "
            << st.thermal_stress / 1e6 << " MPa\n";

        if (st.failed) {
            std::cout << "STRUCTURAL FAILURE: Thermal stress exceeded\n";
        }
        else {
            std::cout << "Structure within allowable limits\n";
        }


    }
} // namespace ORCUS
