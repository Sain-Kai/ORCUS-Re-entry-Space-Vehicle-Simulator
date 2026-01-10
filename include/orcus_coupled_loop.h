#pragma once
#include "orcus_config.h"
#include "orcus_tps.h"
#include "orcus_guidance.h"
#include "orcus_dynamics.h"

namespace ORCUS {

    struct CoupledStepResult {
        double heat_flux;
        double Mach;
        double gamma;
        bool tps_failed;
    };

    CoupledStepResult run_coupled_step(
        State6DOF& s,
        Vehicle6DOF& veh,
        TPSState& tps,
        const TPSMaterial& mat,
        const OrcusConfig& cfg,
        double dt
    );

}
