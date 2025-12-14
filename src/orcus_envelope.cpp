#include "../include/orcus_envelope.h"
#include "../include/orcus_core.h"
#include "../include/orcus_config.h"
#include "../include/orcus_constants.h"

#include <iostream>
#include <cmath>
#include <limits>

namespace ORCUS {

    void run_worst_case_envelope() {

        std::cout << "\nORCUS Phase-3N WORST-CASE ENVELOPE Running...\n";

        constexpr double entry_gammas_deg[] = { -5.0, -6.0, -7.0, -8.0 };
        constexpr double bank_angles_deg[] = { 0.0, 15.0, 30.0, 45.0, 60.0 };
        constexpr double nose_radii[] = { 0.5, 0.75, 1.0, 1.25 };

        EnvelopeResult worst{};
        worst.peak_T_ratio = 0.0;
        worst.remaining_tps = 1.0;
        worst.peak_q = 0.0;
        worst.survived = true;

        for (double gamma0 : entry_gammas_deg) {
            for (double bank_deg : bank_angles_deg) {
                for (double Rn : nose_radii) {

                    OrcusConfig cfg = default_config();

                    // Entry angle control
                    cfg.initial_vz_mps =
                        cfg.initial_speed_mps * std::sin(gamma0 * PI / 180.0);

                    // Geometry control
                    cfg.nose_radius_m = Rn;

                    ThermalSummary s =
                        run_thermal_summary(cfg, bank_deg * PI / 180.0);

                    bool failed =
                        (s.peak_T_ratio >= 1.0) ||
                        (s.remaining_tps <= 0.0);

                    bool worse =
                        (s.peak_T_ratio > worst.peak_T_ratio) ||
                        (s.peak_q > worst.peak_q);

                    if (worse) {
                        worst.entry_gamma_deg = gamma0;
                        worst.bank_deg = bank_deg;
                        worst.nose_radius = Rn;
                        worst.peak_T_ratio = s.peak_T_ratio;
                        worst.remaining_tps = s.remaining_tps;
                        worst.peak_q = s.peak_q;
                        worst.survived = !failed;
                    }

                    std::cout
                        << "3N | gamma=" << gamma0
                        << " bank=" << bank_deg
                        << " Rn=" << Rn
                        << " T/Tmax=" << s.peak_T_ratio
                        << " survived=" << !failed
                        << "\n";
                }
            }
        }

        std::cout << "\n--- WORST-CASE ENVELOPE ---\n";
        std::cout << "Entry gamma (deg): " << worst.entry_gamma_deg << "\n";
        std::cout << "Bank angle (deg) : " << worst.bank_deg << "\n";
        std::cout << "Nose radius (m)  : " << worst.nose_radius << "\n";
        std::cout << "Peak T/Tmax      : " << worst.peak_T_ratio << "\n";
        std::cout << "Remaining TPS    : " << worst.remaining_tps << "\n";
        std::cout << "Peak heat flux   : " << worst.peak_q << "\n";
        std::cout << "Survived         : " << worst.survived << "\n";

        std::cout << "ORCUS Phase-3N completed\n";
    }

}
