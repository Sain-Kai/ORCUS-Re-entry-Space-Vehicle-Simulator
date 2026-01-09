#pragma once

namespace ORCUS {

    struct InviscidPoint {
        double x;        // Surface location (m)
        double Cp;       // Pressure coefficient
        double Ue;       // Edge velocity (m/s)
        double Pe;       // Edge pressure (Pa)
    };

    struct InviscidSolution {
        int N;
        InviscidPoint* pts;
    };

    // Computes inviscid pressure distribution along forebody
    InviscidSolution compute_inviscid_flow(
        double Mach,
        double gamma,
        double V_inf,
        double P_inf,
        double nose_radius,
        int Npts
    );

}
