#pragma once

namespace ORCUS {

struct UncertaintyParams {
    double rho_frac;   // ± density fraction
    double cd_frac;    // ± aerodynamic coefficient
    double heat_frac;  // ± heat flux coefficient
};

struct UncertaintyResult {
    bool survived;
    double worst_T_ratio;
    double worst_remaining_tps;
};

UncertaintyResult run_uncertainty_case(
    double rho,
    double q_conv,
    double T_surface,
    double tps_thickness,
    double Tmax,
    const UncertaintyParams& u
);

}
