#include "../include/orcus_coupled_loop.h"
#include "../include/orcus_physics.h"
#include "../include/orcus_heat.h"
#include "../include/orcus_dynamics.h"

#include <cmath>
#include <algorithm>

namespace ORCUS {

    CoupledStepResult run_coupled_step(
        State6DOF& s,
        Vehicle6DOF& veh,
        TPSState& tps,
        const TPSMaterial& mat,
        const OrcusConfig& cfg,
        double dt
    ) {
        CoupledStepResult out{};

        double V = std::sqrt(s.vx * s.vx + s.vz * s.vz);
        double Mach = V / speed_of_sound(s.z);
        double gamma = std::atan2(-s.vz, s.vx);

        double rho = density(s.z);
        double Rex = rho * V * veh.cref / 1.8e-5;

        HeatFlux H = compute_heating(
            rho, V, cfg.nose_radius_m,
            gamma, Mach, Rex,
            tps.T_surface,
            mat.emissivity
        );

        // Update TPS
        tps = update_tps_ablation(tps, mat, H.q_net, dt);

        // Guidance reacts to HEAT, not just Mach
        GuidanceCmd gcmd =
            skip_guidance(s.z, Mach, H.q_net, gamma, cfg);

        s.vz += V * gcmd.gamma_dot_cmd * dt;

        // Trajectory update
        s = rk4_step_6dof(s, dt, veh);

        out.heat_flux = H.q_net;
        out.Mach = Mach;
        out.gamma = gamma;
        out.tps_failed = tps.failed;

        return out;
    }

}
