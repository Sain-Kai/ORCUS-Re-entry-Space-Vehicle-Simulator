// src/orcus_traj_opt.cpp
// ORCUS Phase-3G : Trajectory Optimization (bank angle sweep)

#include "../include/orcus_traj_opt.h"
#include "../include/orcus_constants.h"
#include "../include/orcus_dynamics.h"
#include "../include/orcus_physics.h"

#include <iostream>
#include <cmath>

namespace ORCUS {

    void run_traj_optimization() {

        std::cout << "ORCUS Phase-3G TRAJ-OPT Running...\n";

        // Sweep bank angles (deg)
        const double bank_angles[] = { 0, 15, 30, 45, 60 };

        for (double bank_deg : bank_angles) {

            Vehicle6DOF veh{};
            veh.mass = 1500.0;
            veh.Sref = 1.8;
            veh.cref = 2.0;
            veh.bank = bank_deg * PI / 180.0;   // ✅ FIXED (NO M_PI)
            veh.Ixx = 800;
            veh.Iyy = 1200;
            veh.Izz = 1000;

            State6DOF s{};
            s.x = 0;
            s.y = 0;
            s.z = 120000;
            s.vx = 7800;
            s.vy = 0;
            s.vz = -50;
            s.q0 = 1;
            s.q1 = s.q2 = s.q3 = 0;
            s.p = s.q = s.r = 0;

            double t = 0.0;
            double dt = 0.05;
            bool survived = true;

            while (s.z > 0 && t < 2000.0) {
                s = rk4_step_6dof(s, dt, veh);

                double V = std::sqrt(s.vx * s.vx + s.vy * s.vy + s.vz * s.vz);
                double Mach = V / speed_of_sound(s.z);

                if (Mach < 0.3 || V < 300) break;  // terminal regime

                if (std::isnan(s.z) || std::isnan(V)) {
                    survived = false;
                    break;
                }

                t += dt;
            }

            std::cout << "TRAJ-OPT | bank=" << bank_deg
                << " survived=" << survived << "\n";
        }

        std::cout << "TRAJ-OPT completed\n";
    }

} // namespace ORCUS
