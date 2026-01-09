#include "../include/orcus_real_gas.h"
#include <algorithm>

namespace ORCUS {

    RealGasProps compute_real_gas_props(double T) {

        RealGasProps rg{};

        // -------------------------------------------------
        // Temperature-dependent Cp (J/kg-K)
        // Piecewise NASA-style engineering fit
        // -------------------------------------------------
        if (T < 1000.0) {
            rg.Cp = 1005.0;
        }
        else if (T < 3000.0) {
            rg.Cp = 1005.0 + 0.1 * (T - 1000.0);
        }
        else {
            rg.Cp = 1200.0;
        }

        // -------------------------------------------------
        // Temperature-dependent gamma
        // Accounts for vibrational excitation
        // -------------------------------------------------
        if (T < 1000.0) {
            rg.gamma = 1.40;
        }
        else if (T < 3000.0) {
            rg.gamma = 1.40 - 0.0001 * (T - 1000.0);
        }
        else {
            rg.gamma = 1.20;
        }

        // Safety clamp
        rg.gamma = std::clamp(rg.gamma, 1.15, 1.40);

        return rg;
    }

}
