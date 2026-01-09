#pragma once

namespace ORCUS {

    struct ShockLayerProps {
        double thickness;   // Shock-layer thickness (m)
        double efficiency;  // Heat-flux attenuation factor (0–1)
    };

    // Computes finite shock-layer thickness and efficiency factor
    // based on blunt-body hypersonic theory
    ShockLayerProps compute_shock_layer(
        double Mach_inf,     // Freestream Mach number
        double gamma,        // Ratio of specific heats
        double nose_radius   // Nose radius (m)
    );

}
