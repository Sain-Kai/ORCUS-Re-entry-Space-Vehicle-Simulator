#pragma once

namespace ORCUS {

    // -------- User-controlled simulation parameters --------
    struct OrcusConfig {

        // Entry conditions
        double initial_altitude_m;
        double initial_speed_mps;
        double initial_vz_mps;

        // Geometry
        double nose_radius_m;

        // TPS
        double tps_thickness_m;

        // Guidance / skip control
        double heat_flux_limit;
        double gamma_pullup_rate;
        double gamma_dive_rate;

    };

    // Returns default (safe, realistic) config
    OrcusConfig default_config();

}
