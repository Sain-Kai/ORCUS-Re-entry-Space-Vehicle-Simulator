#include "../include/orcus_inviscid.h"
#include "../include/orcus_constants.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

namespace ORCUS {

    InviscidSolution compute_inviscid_flow(
        double Mach,
        double gamma,
        double V_inf,
        double P_inf,
        double Rn,
        int Npts
    ) {
        InviscidSolution sol{};
        sol.N = Npts;
        sol.pts = new InviscidPoint[Npts];

        // Modified Newtonian stagnation Cp
        double Cp_max =
            2.0 / (gamma * Mach * Mach) *
            (std::pow(1.0 + 0.5 * (gamma - 1.0) * Mach * Mach,
                gamma / (gamma - 1.0)) - 1.0);

        for (int i = 0; i < Npts; ++i) {

            double theta =
                (double)i / (Npts - 1) * (PI / 2.0);

            double Cp =
                Cp_max * std::pow(std::sin(theta), 2.0);

            double Pe =
                P_inf * (1.0 + Cp * 0.5 * gamma * Mach * Mach);

            double Ue =
                V_inf * std::sqrt(std::max(0.0, 1.0 - Cp));

            sol.pts[i] = {
                Rn * theta,
                Cp,
                Ue,
                Pe
            };
        }

        return sol;
    }

}
