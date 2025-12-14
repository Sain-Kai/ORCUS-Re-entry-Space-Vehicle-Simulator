#include "../include/orcus_guidance.h"
#include <cmath>

namespace ORCUS {

    static constexpr double PI = 3.14159265358979323846;
    static constexpr double GAMMA_MAX = 5.0 * PI / 180.0;
    static constexpr double GAMMA_MIN = -20.0 * PI / 180.0;

    GuidanceCmd skip_guidance(
        double altitude,
        double Mach,
        double heat_flux,
        double gamma,
        const OrcusConfig& cfg
    ) {
        GuidanceCmd cmd{};
        cmd.gamma_dot_cmd = 0.0;

        // Heating-driven pull-up
        if (heat_flux > cfg.heat_flux_limit && altitude < 95000.0) {
            cmd.gamma_dot_cmd = cfg.gamma_pullup_rate;
        }
        // Controlled descent when safe
        else if (heat_flux < 0.5 * cfg.heat_flux_limit && Mach > 8.0) {
            cmd.gamma_dot_cmd = cfg.gamma_dive_rate;
        }

        // Safety clamps
        if (gamma > GAMMA_MAX && cmd.gamma_dot_cmd > 0.0)
            cmd.gamma_dot_cmd = 0.0;

        if (gamma < GAMMA_MIN && cmd.gamma_dot_cmd < 0.0)
            cmd.gamma_dot_cmd = 0.0;

        return cmd;
    }

}
