#include "../include/orcus_config.h"

namespace ORCUS {

    OrcusConfig default_config() {

        OrcusConfig cfg{};

        // --- Entry ---
        cfg.initial_altitude_m = 120000.0;
        cfg.initial_speed_mps = 7800.0;
        cfg.initial_vz_mps = -35.0;   // TUNING STEP 3

        // --- Geometry ---
        cfg.nose_radius_m = 1.0;

        // --- TPS ---
        cfg.tps_thickness_m = 0.15;       // TUNING STEP 4

        // --- Guidance (Phase-3H tuned) ---
        cfg.heat_flux_limit = 4.0e6;     // TUNING STEP 2
        cfg.gamma_pullup_rate = 0.08;     // TUNING STEP 1
        cfg.gamma_dive_rate = -0.01;

        return cfg;
    }

}
