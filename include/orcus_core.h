#pragma once
#include "orcus_config.h"

namespace ORCUS {

    struct ThermalSummary {
        double peak_T_ratio;
        double remaining_tps;
        double peak_q;
    };

    // Default entrypoint (used by main.cpp)
    void run_default_simulation();

    // Core thermal evaluator (used by Phase-3K, 3M, 3N, etc.)
    ThermalSummary run_thermal_summary(
        const OrcusConfig& cfg,
        double bank_rad
    );

}
