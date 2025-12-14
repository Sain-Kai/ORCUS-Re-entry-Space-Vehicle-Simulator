#pragma once

namespace ORCUS {

    struct Geometry {
        double length;
        double diameter;

        double frontal_area() const;
        double ref_length() const;
        double wetted_area() const;
    };

}
