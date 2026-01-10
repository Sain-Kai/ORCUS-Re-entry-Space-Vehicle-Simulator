#pragma once
#include <vector>

namespace ORCUS {

    struct SurfacePoint {
        double x;      // distance from stagnation point (m)
        double Cp;     // pressure coefficient
        double q;      // heat flux (W/m^2)
    };

    struct SurfaceDistribution {
        std::vector<SurfacePoint> points;
        double peak_q;
        double peak_x;
    };

    SurfaceDistribution compute_surface_distribution(
        double Cp_stag,
        double q_stag,
        double effective_radius,
        double body_length,
        int N
    );

}
