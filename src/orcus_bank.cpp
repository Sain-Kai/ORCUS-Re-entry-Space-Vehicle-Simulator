#include "../include/orcus_bank.h"
#include <cmath>

namespace ORCUS {

    double q_sutton_graves(double rho, double v, double Rn, double C) {
        if (Rn <= 0.0) Rn = 1e-6;
        return C * std::sqrt(rho / Rn) * v * v * v;
    }

}
