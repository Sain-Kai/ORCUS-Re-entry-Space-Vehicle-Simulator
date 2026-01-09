#pragma once

namespace ORCUS {

    struct RealGasProps {
        double gamma;   // Effective ratio of specific heats
        double Cp;      // Specific heat at constant pressure (J/kg-K)
    };

    // Temperature-dependent real-gas model (pre-chemistry)
    RealGasProps compute_real_gas_props(
        double temperature   // Local gas temperature (K)
    );

}
