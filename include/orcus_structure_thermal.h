#pragma once

namespace ORCUS {

    struct StructureThermalProps {
        double T_structure;     // K
        double thermal_stress;  // Pa
        bool failed;
    };

    StructureThermalProps compute_structure_thermal(
        double q_wall,          // W/m^2
        double dt,              // s
        double k_interface,     // W/m-K
        double thickness,       // m
        double rho,             // kg/m^3
        double cp,              // J/kg-K
        double E,               // Pa
        double alpha,           // 1/K
        double T_ref,           // K
        double sigma_allow      // Pa
    );

}
