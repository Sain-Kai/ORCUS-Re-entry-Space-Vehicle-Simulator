#include "../include/orcus_physics.h"
#include "../include/orcus_constants.h"
#include <cmath>

namespace ORCUS {

    double temperature(double h) {
        if (h < 0) h = 0;
        if (h <= tropopause)
            return T0_sea + lapse * h;
        return T0_sea + lapse * tropopause;
    }

    double density(double h) {
        if (h < 0) h = 0;
        if (h <= tropopause) {
            double T = temperature(h);
            double expn = (-g0) / (Rgas * lapse);
            return rho0 * std::pow(T / T0_sea, expn);
        }
        double Tt = temperature(tropopause);
        double expn = (-g0) / (Rgas * lapse);
        double rho_t = rho0 * std::pow(Tt / T0_sea, expn);
        return rho_t * std::exp(-(h - tropopause) / 7500.0);
    }

    double gravity(double h) {
        return g0 * (Re_earth / (Re_earth + h)) *
            (Re_earth / (Re_earth + h));
    }

    double speed_of_sound(double h) {
        return std::sqrt(gamma_air * Rgas * temperature(h));
    }

}
