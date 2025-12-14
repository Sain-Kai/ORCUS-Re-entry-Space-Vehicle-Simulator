#pragma once

namespace ORCUS {

    struct HeatFlux {
        double q_conv;   // W/m^2
        double q_rad;    // W/m^2
        double q_emit;   // W/m^2
        double q_net;    // W/m^2 (into TPS)
    };

    HeatFlux compute_heating(
        double rho,
        double velocity,
        double nose_radius,
        double gamma,
        double Mach,
        double Re_x,
        double T_surface,
        double emissivity
    );

}
