# 🛰️ ORCUS — Hypersonic Re-Entry Vehicle Simulator

---

## Summary 

**ORCUS** is a modular, physics-based **hypersonic atmospheric re-entry vehicle simulator** developed to study trajectory evolution, aerothermal loads, and thermal protection system (TPS) survivability under extreme flight conditions.

The simulator prioritizes **model transparency, staged physical fidelity, and failure-aware design**, aligning with workflows used in preliminary aerospace vehicle analysis rather than black-box visualization tools.

---

## Scientific Objectives

* Model hypersonic re-entry trajectories using numerically stable solvers
* Estimate stagnation-point heating and total heat load
* Simulate TPS temperature rise and failure conditions
* Provide a platform extensible toward CFD / wind-tunnel validated inputs

---

## Current Capabilities (v0.3)

### Flight Dynamics

* 3-DOF point-mass re-entry equations
* Gravity-turn based trajectory propagation
* Mach-number-dependent regime handling
* 4th-order Runge–Kutta (RK4) integration

### Atmosphere & Aerodynamics

* Exponential atmospheric density model
* Mach-dependent drag coefficient (piecewise)
* Dynamic pressure computation
* Hypersonic drag-dominated assumptions

### Aerothermal Modeling

* Sutton–Graves stagnation-point heating model
* Parametric Fay–Riddell-style baseline (non-equilibrium chemistry excluded)
* Convective heat flux estimation
* Time-integrated heat load tracking

### Thermal Protection System (TPS)

* Abstract TPS material model
* Temperature rise from applied heat flux
* Thermal closure phase
* TPS failure detection and mission termination

---

## Scientific Assumptions & Scope

To maintain numerical stability and architectural clarity, ORCUS currently adopts the following assumptions:

1. **Continuum flow regime** (no rarefied flow modeling)
2. **Frozen chemistry** (no dissociation or ionization)
3. **Stagnation-point heating dominance**
4. **No surface temperature feedback into aerodynamics**
5. **Axisymmetric vehicle assumption**
6. **No guidance or control feedback loops** (open-loop trajectory)

These assumptions are explicitly documented to support academic review, validation planning, and future fidelity upgrades.

---

## Architecture Overview

```
ORCUS/
│
├── src/
│   ├── core/        # Trajectory driver & mission logic
│   ├── physics/     # Atmosphere, aero, heat flux, constants
│   ├── thermal/     # TPS & thermal closure
│   ├── solver/      # RK4 integrator
│   └── state/       # Vehicle state definitions
│
├── docs/
├── assets/
└── README.md
```

---

## Versioning Policy

ORCUS follows **semantic-style staged versioning**:

* **0.x** → Research & architecture stabilization
* **1.0** → Validated physics + visualization
* **2.0** → Guidance, control, and optimization

Detailed change history is maintained in `CHANGELOG.md`.

---

## Build Instructions

### Windows (Visual Studio)

* Open `ORCUS.sln`
* Build: `x64 | Release`
* Run executable

### GCC / MinGW

```bash
g++ -std=c++17 -O2 src/**/*.cpp -Iinclude -o orcus
./orcus
```

---

## License

MIT License — see `LICENSE.md`

---

## Author

**Sohan Manna**
Lead Developer — ORCUS
Hypersonic Flight • Aerothermodynamics • Simulation Architecture

---

> **ORCUS is intentionally engineered as a closing physical system — where assumptions, limits, and failures are as important as successful trajectories.**
