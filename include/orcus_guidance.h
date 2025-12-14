#pragma once
#include "orcus_config.h"

namespace ORCUS {

    struct GuidanceCmd {
        double gamma_dot_cmd; // rad/s
    };

    GuidanceCmd skip_guidance(
        double altitude,
        double Mach,
        double heat_flux,
        double gamma,
        const OrcusConfig& cfg
    );

}
