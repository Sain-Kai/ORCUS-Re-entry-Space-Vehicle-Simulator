#include "../include/orcus_pns_energy.h"

namespace ORCUS {

    PNSEnergyResult solve_pns_energy(
        double T_edge,
        double T_wall,
        double delta_e,
        double k_air
    ) {
        PNSEnergyResult r{};

        // Linearized wall-normal temperature profile
        r.dTdy_wall = (T_edge - T_wall) / delta_e;

        // Fourier's law
        r.q_wall = -k_air * r.dTdy_wall;

        return r;
    }

}
