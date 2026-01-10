#include "../include/orcus_structure_thermal.h"
#include <algorithm>

namespace ORCUS {

    StructureThermalProps compute_structure_thermal(
        double q_wall,
        double dt,
        double k_interface,
        double thickness,
        double rho,
        double cp,
        double E,
        double alpha,
        double T_ref,
        double sigma_allow
    ) {
        StructureThermalProps s{};

        // Temperature rise (lumped)
        double dT =
            (q_wall * dt) /
            (rho * cp * thickness);

        s.T_structure = T_ref + dT;

        // Thermal stress
        s.thermal_stress =
            E * alpha * (s.T_structure - T_ref);

        s.failed = (s.thermal_stress >= sigma_allow);

        return s;
    }

}
