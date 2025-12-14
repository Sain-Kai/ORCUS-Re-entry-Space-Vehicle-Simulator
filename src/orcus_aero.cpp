#include "../include/orcus_aero.h"
#include <cmath>

namespace ORCUS {

    AeroCoeffs aero_coefficients(double alpha, double Mach) {

        double Cd0 = 1.2 + 0.15 * std::log(Mach + 1.0);
        double Cla = 2.0 / std::sqrt(Mach * Mach - 1.0);

        AeroCoeffs c{};
        c.Cl = Cla * alpha;
        c.Cd = Cd0 + 0.6 * alpha * alpha;

        // Static stability: negative Cm_alpha
        c.Cm = -0.05 * alpha;

        return c;
    }

    AeroDerivatives aero_derivatives(double Mach) {
        AeroDerivatives d{};
        d.Cm_alpha = -0.05;
        d.Cm_q = -8.0 / (Mach + 1.0);
        d.Cl_p = -0.6;
        d.Cn_r = -0.6;
        return d;
    }

}
