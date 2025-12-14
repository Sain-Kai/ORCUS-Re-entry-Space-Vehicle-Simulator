#include "../include/orcus_geom.h"
#include "../include/orcus_constants.h"
#include <algorithm>

namespace ORCUS {

    double Geometry::frontal_area() const {
        double r = diameter * 0.5;
        return PI * r * r;
    }

    double Geometry::ref_length() const {
        return std::max(length, diameter);
    }

    double Geometry::wetted_area() const {
        double r = diameter * 0.5;
        double cyl = 2.0 * PI * r * (length - r);
        double nose = 2.0 * PI * r * r;
        return std::max(0.0, cyl) + nose;
    }

}
