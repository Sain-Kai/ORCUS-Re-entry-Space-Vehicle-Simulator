#include "../include/orcus_core.h"
#include "../include/orcus_dynamics.h"
#include "../include/orcus_physics.h"
#include "../include/orcus_heat.h"
#include "../include/orcus_tps.h"
#include "../include/orcus_guidance.h"
#include "../include/orcus_constants.h"
#include "../include/orcus_config.h"

#include <iostream>
#include <cmath>
#include <algorithm>

namespace ORCUS {

    // ============================================================
    // CORE THERMAL SUMMARY ENGINE (used by 3K / 3M / 3N)
    // ============================================================
    ThermalSummary run_thermal_summary(
        const OrcusConfig& cfg,
        double bank_rad
    ) {
        ThermalSummary out{};
        out.peak_T_ratio = 0.0;
        out.remaining_tps = 1.0;
        out.peak_q = 0.0;

        Vehicle6DOF veh{};
        veh.mass = 1500.0;
        veh.Sref = 1.8;
        veh.cref = 2.0;
        veh.bank = bank_rad;
        veh.Ixx = 800.0;
        veh.Iyy = 1200.0;
        veh.Izz = 1000.0;

        TPSMaterial tps{};
        tps.density = 1600.0;
        tps.cp = 1250.0;
        tps.k = 0.15;
        tps.emissivity = 0.9;
        tps.Tmax = 2200.0;
        tps.L_abl = 2.5e6;

        TPSState tps_state{};
        tps_state.T_surface = 300.0;
        tps_state.T_bulk = 300.0;
        tps_state.thickness = cfg.tps_thickness_m;
        tps_state.failed = false;

        State6DOF s{};
        s.x = 0.0;
        s.y = 0.0;
        s.z = cfg.initial_altitude_m;
        s.vx = cfg.initial_speed_mps;
        s.vy = 0.0;
        s.vz = cfg.initial_vz_mps;
        s.q0 = 1.0;
        s.q1 = s.q2 = s.q3 = 0.0;
        s.p = s.q = s.r = 0.0;

        const double dt = 0.02;

        while (s.z > 0.0 && !tps_state.failed) {

            double V = std::sqrt(s.vx * s.vx + s.vy * s.vy + s.vz * s.vz);
            double Mach = V / speed_of_sound(s.z);
            double gamma = std::atan2(-s.vz, s.vx);

            double rho = density(s.z);
            double Rex = rho * V * veh.cref / 1.8e-5;

            HeatFlux H = compute_heating(
                rho,
                V,
                cfg.nose_radius_m,
                gamma,
                Mach,
                Rex,
                tps_state.T_surface,
                tps.emissivity
            );

            out.peak_q = std::max(out.peak_q, H.q_net);

            tps_state = update_tps_ablation(
                tps_state,
                tps,
                H.q_net,
                dt
            );

            out.peak_T_ratio = std::max(
                out.peak_T_ratio,
                tps_state.T_surface / tps.Tmax
            );

            out.remaining_tps = std::min(
                out.remaining_tps,
                tps_state.thickness / cfg.tps_thickness_m
            );

            GuidanceCmd gcmd = skip_guidance(
                s.z,
                Mach,
                H.q_net,
                gamma,
                cfg
            );

            s.vz += V * gcmd.gamma_dot_cmd * dt;
            s = rk4_step_6dof(s, dt, veh);
        }

        return out;
    }

    // ============================================================
    // REQUIRED ENTRY POINT (FIXES LNK2019)
    // ============================================================
    void run_default_simulation() {

        std::cout << "ORCUS Phase-3 Engine Running\n";
        std::cout << "ORCUS Phase-3K THERMAL MARGIN QUANTIFICATION Running...\n\n";

        OrcusConfig cfg = default_config();

        // Nominal bank angle (certification baseline)
        double bank_rad = 0.35;

        ThermalSummary r = run_thermal_summary(cfg, bank_rad);

        std::cout << "--- Phase-3K Thermal Margins ---\n";
        std::cout << "Peak T/Tmax           : " << r.peak_T_ratio << "\n";
        std::cout << "Remaining TPS fraction: " << r.remaining_tps << "\n";
        std::cout << "Peak heat flux margin : " << r.peak_q << "\n\n";

        if (r.peak_T_ratio >= 1.0 || r.remaining_tps <= 0.0)
            std::cout << "TPS FAILURE after thermal margin evaluation\n";
        else
            std::cout << "ORCUS Phase-3K SUCCESS\n";
    }

}
