#include "../include/orcus_dynamics.h"
#include "../include/orcus_physics.h"
#include "../include/orcus_aero.h"
#include <cmath>

namespace ORCUS {

    static void body_to_inertial(
        const State6DOF& s,
        double bx, double by, double bz,
        double& ix, double& iy, double& iz
    ) {
        ix = (1 - 2 * (s.q2 * s.q2 + s.q3 * s.q3)) * bx
            + 2 * (s.q1 * s.q2 - s.q0 * s.q3) * by
            + 2 * (s.q1 * s.q3 + s.q0 * s.q2) * bz;

        iy = 2 * (s.q1 * s.q2 + s.q0 * s.q3) * bx
            + (1 - 2 * (s.q1 * s.q1 + s.q3 * s.q3)) * by
            + 2 * (s.q2 * s.q3 - s.q0 * s.q1) * bz;

        iz = 2 * (s.q1 * s.q3 - s.q0 * s.q2) * bx
            + 2 * (s.q2 * s.q3 + s.q0 * s.q1) * by
            + (1 - 2 * (s.q1 * s.q1 + s.q2 * s.q2)) * bz;
    }

    static State6DOF derivs(const State6DOF& s, const Vehicle6DOF& v) {

        State6DOF d{};
        double rho = density(s.z);
        double V = std::sqrt(s.vx * s.vx + s.vy * s.vy + s.vz * s.vz);
        if (V < 1e-6) V = 1e-6;

        double alpha = std::atan2(-s.vz, s.vx);
        double Mach = V / speed_of_sound(s.z);

        AeroCoeffs C = aero_coefficients(alpha, Mach);
        AeroDerivatives D = aero_derivatives(Mach);

        double qbar = 0.5 * rho * V * V;

        double Drag = qbar * C.Cd * v.Sref;
        double Lift = qbar * C.Cl * v.Sref;

        double Fx_b = -Drag;
        double Fy_b = Lift * std::sin(v.bank);
        double Fz_b = Lift * std::cos(v.bank);

        double Fx, Fy, Fz;
        body_to_inertial(s, Fx_b, Fy_b, Fz_b, Fx, Fy, Fz);

        d.vx = Fx / v.mass;
        d.vy = Fy / v.mass;
        d.vz = Fz / v.mass - gravity(s.z);

        d.x = s.vx;
        d.y = s.vy;
        d.z = s.vz;

        // Moments
        double My = qbar * v.Sref * v.cref *
            (C.Cm + D.Cm_q * (s.q * v.cref / (2 * V)));

        double Mx = qbar * v.Sref * v.cref * (D.Cl_p * s.p);
        double Mz = qbar * v.Sref * v.cref * (D.Cn_r * s.r);

        d.p = (Mx - (v.Izz - v.Iyy) * s.q * s.r) / v.Ixx;
        d.q = (My - (v.Ixx - v.Izz) * s.p * s.r) / v.Iyy;
        d.r = (Mz - (v.Iyy - v.Ixx) * s.p * s.q) / v.Izz;

        d.q0 = -0.5 * (s.q1 * s.p + s.q2 * s.q + s.q3 * s.r);
        d.q1 = 0.5 * (s.q0 * s.p + s.q2 * s.r - s.q3 * s.q);
        d.q2 = 0.5 * (s.q0 * s.q - s.q1 * s.r + s.q3 * s.p);
        d.q3 = 0.5 * (s.q0 * s.r + s.q1 * s.q - s.q2 * s.p);

        return d;
    }

    State6DOF rk4_step_6dof(
        const State6DOF& s,
        double dt,
        const Vehicle6DOF& v
    ) {
        auto k1 = derivs(s, v);

        State6DOF s2 = s;
#define STEP(a) s2.a = s.a + 0.5*dt*k1.a
        STEP(x); STEP(y); STEP(z);
        STEP(vx); STEP(vy); STEP(vz);
        STEP(p); STEP(q); STEP(r);
        STEP(q0); STEP(q1); STEP(q2); STEP(q3);
#undef STEP

        auto k2 = derivs(s2, v);

        State6DOF s3 = s;
#define STEP(a) s3.a = s.a + 0.5*dt*k2.a
        STEP(x); STEP(y); STEP(z);
        STEP(vx); STEP(vy); STEP(vz);
        STEP(p); STEP(q); STEP(r);
        STEP(q0); STEP(q1); STEP(q2); STEP(q3);
#undef STEP

        auto k3 = derivs(s3, v);

        State6DOF s4 = s;
#define STEP(a) s4.a = s.a + dt*k3.a
        STEP(x); STEP(y); STEP(z);
        STEP(vx); STEP(vy); STEP(vz);
        STEP(p); STEP(q); STEP(r);
        STEP(q0); STEP(q1); STEP(q2); STEP(q3);
#undef STEP

        auto k4 = derivs(s4, v);

        State6DOF out = s;
#define COMB(a) out.a += dt*(k1.a + 2*k2.a + 2*k3.a + k4.a)/6.0
        COMB(x); COMB(y); COMB(z);
        COMB(vx); COMB(vy); COMB(vz);
        COMB(p); COMB(q); COMB(r);
        COMB(q0); COMB(q1); COMB(q2); COMB(q3);
#undef COMB

        return out;
    }

}
