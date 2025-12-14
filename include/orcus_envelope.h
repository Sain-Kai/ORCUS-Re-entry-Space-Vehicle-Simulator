#pragma once

namespace ORCUS {

    struct EnvelopeResult {
        double entry_gamma_deg;
        double bank_deg;
        double nose_radius;

        double peak_T_ratio;
        double remaining_tps;
        double peak_q;

        bool survived;
    };

    void run_worst_case_envelope();

}
