#pragma once

namespace ORCUS {

    struct State6DOF {
        double x, y, z;
        double vx, vy, vz;

        double q0, q1, q2, q3;

        double p, q, r;
    };

    struct Vehicle6DOF {
        double mass;
        double Sref;
        double cref;
        double bank;

        double Ixx, Iyy, Izz;
    };

    State6DOF rk4_step_6dof(
        const State6DOF& s,
        double dt,
        const Vehicle6DOF& veh
    );

}
