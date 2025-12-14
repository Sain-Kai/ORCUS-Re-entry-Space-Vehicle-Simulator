#include "../include/orcus_tps.h"
#include <cmath>
#include <algorithm>

namespace ORCUS {

    static constexpr double sigma = 5.670374419e-8;

    TPSState update_tps_ablation(
        const TPSState& s,
        const TPSMaterial& mat,
        double q_in,
        double dt
    ) {
        TPSState out = s;

        // -------- Phase-3K peak tracking --------
        out.max_T_surface = std::max(out.max_T_surface, s.T_surface);
        out.max_q_net = std::max(out.max_q_net, q_in);

        if (out.thickness <= 0.0) {
            out.failed = true;
            return out;
        }

        double A = 1.0;
        double ms = mat.density * out.thickness * A;
        double mb = ms * 2.0;

        double q_rad = mat.emissivity * sigma * std::pow(s.T_surface, 4);
        double q_cond = mat.k * (s.T_surface - s.T_bulk) / out.thickness;

        double q_net = q_in - q_rad - q_cond;

        double mdot_pp = std::max(0.0, q_net / mat.L_abl);
        double dthickness = (mdot_pp / mat.density) * dt;

        out.thickness -= dthickness;
        if (out.thickness < 0.0) out.thickness = 0.0;

        double dTs = (q_net * dt) / (ms * mat.cp);
        double dTb = (q_cond * dt) / (mb * mat.cp);

        out.T_surface += dTs;
        out.T_bulk += dTb;

        out.failed =
            (out.T_surface > mat.Tmax) ||
            (out.thickness <= 0.0);

        return out;
    }

}
