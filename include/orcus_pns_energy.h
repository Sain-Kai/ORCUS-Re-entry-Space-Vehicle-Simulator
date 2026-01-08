#pragma once

namespace ORCUS {

    struct PNSEnergyResult {
        double dTdy_wall;   // temperature gradient at wall
        double q_wall;      // Fourier heat flux
    };

    PNSEnergyResult solve_pns_energy(
        double T_edge,
        double T_wall,
        double delta_e,
        double k_air
    );

}
