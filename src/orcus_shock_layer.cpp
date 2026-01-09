#include "../include/orcus_shock_layer.h"
#include <cmath>
#include <algorithm>

namespace ORCUS {

    ShockLayerProps compute_shock_layer(
        double Mach_inf,
        double gamma,
        double nose_radius
    ) {
        ShockLayerProps sl{};

        // --------------------------------------------
        // Shock-layer thickness model (blunt body)
        // δ_s ≈ C_s * R_n * ((γ−1)M² + 2) / ((γ+1)(M²−1))
        // --------------------------------------------
        constexpr double C_s = 0.8;  // Empirical constant (NASA SP-8007)

        double M2 = Mach_inf * Mach_inf;

        double numerator =
            (gamma - 1.0) * M2 + 2.0;

        double denominator =
            (gamma + 1.0) * std::max(M2 - 1.0, 1e-6);

        sl.thickness =
            C_s * nose_radius * (numerator / denominator);

        // --------------------------------------------
        // Shock-layer efficiency factor
        // Models attenuation of wall heat flux
        // --------------------------------------------
        double L_ref = nose_radius;  // Characteristic length

        sl.efficiency =
            std::exp(-sl.thickness / std::max(L_ref, 1e-6));

        // Clamp for numerical safety
        sl.efficiency =
            std::clamp(sl.efficiency, 0.0, 1.0);

        return sl;
    }

}
