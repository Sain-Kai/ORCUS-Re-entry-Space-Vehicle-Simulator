#pragma once

namespace ORCUS {

struct TPSOptResult {
    double thickness;
    double nose_radius;
    bool survived;
    double peak_T;
    double ablated;
    double remaining;
};

void run_tps_optimization();

}
