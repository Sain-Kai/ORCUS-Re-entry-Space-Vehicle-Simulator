#pragma once

namespace ORCUS {

    struct TPSState {
        double T_surface;
        double T_bulk;
        double thickness;
        bool failed;

        // -------- Phase-3K additions --------
        double max_T_surface = 0.0;
        double max_q_net = 0.0;
    };

    struct TPSMaterial {
        double density;       // kg/m^3
        double cp;            // J/kg-K
        double k;             // W/m-K
        double emissivity;
        double Tmax;          // K
        double L_abl;         // J/kg (heat of ablation)
    };

    TPSState update_tps_ablation(
        const TPSState& s,
        const TPSMaterial& mat,
        double q_in,
        double dt
    );

}
