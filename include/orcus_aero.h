#pragma once

namespace ORCUS {

    struct AeroCoeffs {
        double Cd;
        double Cl;
        double Cm;
    };

    struct AeroDerivatives {
        double Cm_alpha;
        double Cm_q;
        double Cl_p;
        double Cn_r;
    };

    AeroCoeffs aero_coefficients(double alpha, double Mach);
    AeroDerivatives aero_derivatives(double Mach);

}
