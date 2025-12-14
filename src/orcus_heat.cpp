#include "../include/orcus_heat.h"
#include <cmath>
#include <algorithm>

namespace ORCUS {

    static constexpr double SIGMA = 5.670374419e-8;
    static constexpr double C_SG = 1.83e-4;

    HeatFlux compute_heating(
        double rho,
        double velocity,
        double nose_radius,
        double gamma,
        double Mach,
        double Re_x,
        double T_surface,
        double emissivity
    ) {
        HeatFlux H{};

        // Convective heating (Sutton–Graves)
        H.q_conv =
            C_SG *
            std::sqrt(rho / std::max(nose_radius, 1e-6)) *
            velocity * velocity * velocity;

        // Radiative heating (simplified hypersonic model)
        if (Mach > 12.0) {
            H.q_rad = 5.0e-5 * rho * std::pow(velocity, 3.2);
        }
        else {
            H.q_rad = 0.0;
        }

        // Surface re-radiation
        H.q_emit = emissivity * SIGMA * std::pow(T_surface, 4.0);

        // Net heat into TPS
        H.q_net = std::max(0.0, H.q_conv + H.q_rad - H.q_emit);

        return H;
    }

}
