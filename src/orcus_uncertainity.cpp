#include "../include/orcus_uncertainity.h"
#include <algorithm>

namespace ORCUS {

UncertaintyResult run_uncertainty_case(
    double rho,
    double q_conv,
    double T_surface,
    double tps_thickness,
    double Tmax,
    const UncertaintyParams& u
) {
    UncertaintyResult r{};
    r.survived = true;

    // Worst-case stacking (conservative)
    double rho_eff = rho * (1.0 + u.rho_frac);
    double q_eff = q_conv * (1.0 + u.cd_frac) * (1.0 + u.heat_frac);

    // Thermal margin
    double T_ratio = T_surface / Tmax;

    // TPS erosion proxy (simple worst-case scaling)
    double remaining_frac = std::max(
        0.0,
        tps_thickness * (1.0 - u.heat_frac - u.cd_frac)
    ) / tps_thickness;

    r.worst_T_ratio = T_ratio;
    r.worst_remaining_tps = remaining_frac;

    if (T_ratio >= 1.0 || remaining_frac <= 0.0) {
        r.survived = false;
    }

    return r;
}

}
