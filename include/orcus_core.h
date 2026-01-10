#pragma once
#include "orcus_config.h"
#include "orcus_tps.h"

namespace ORCUS {

    enum class OrcusStage {
        PHASE_3K,
        PHASE_3N,
        PHASE_3O,
        PHASE_3P,
		PHASE_3Q,
		PHASE_3W,
        PHASE_4C_1,
        PHASE_4C_2,
		PHASE_4C_3,
		PHASE_4C_4,
		PHASE_4C_5,
		PHASE_4D,
		PHASE_4E,
        PHASE_4F,
        PHASE_4G,
        PHASE_5A,
        PHASE_5B,
        PHASE_5C,
        PHASE_5D,
        PHASE_5E,
		PHASE_5F,
		PHASE_5G,
        PHASE_6A,
		PHASE_6B,
		PHASE_6C,
		PHASE_6D,
		PHASE_6E,
        PHASE_7A
    };

    struct ThermalSummary {
        double peak_T_ratio;
        double remaining_tps;
        double peak_q;
        TPSFailureMode failure_mode;
    };

    void run_default_simulation();

    ThermalSummary run_thermal_summary(
        const OrcusConfig& cfg,
        double bank_rad
    );

    void print_stage_banner(OrcusStage stage);
}
