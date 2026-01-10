#include "../include/orcus_surface_distribution.h"
#include <cmath>
#include <algorithm>

namespace ORCUS {

    SurfaceDistribution compute_surface_distribution(
        double Cp_stag,
        double q_stag,
        double effective_radius,
        double body_length,
        int N
    ) {
        SurfaceDistribution dist{};
        dist.peak_q = 0.0;
        dist.peak_x = 0.0;

        for (int i = 0; i <= N; i++) {

            double x = body_length * i / N;

            // Pressure coefficient decay
            double Cp =
                Cp_stag * std::exp(-x / effective_radius);

            // Heat flux decay (hypersonic BL scaling)
            double q =
                q_stag *
                std::pow(
                    effective_radius / (x + effective_radius),
                    1.5
                );

            dist.points.push_back({ x, Cp, q });

            if (q > dist.peak_q) {
                dist.peak_q = q;
                dist.peak_x = x;
            }
        }

        return dist;
    }

}
