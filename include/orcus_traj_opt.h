#pragma once

namespace ORCUS {

    struct TrajOptResult {
        double bank_deg;
        bool survived;
        double peak_heat;
        double remaining_tps;
    };

    void run_traj_optimization();

}
