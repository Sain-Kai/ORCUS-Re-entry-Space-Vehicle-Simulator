# 🛰️ ORCUS — Hypersonic Re-Entry Vehicle Simulator

**ORCUS** is a modular, physics-driven hypersonic atmospheric re-entry vehicle simulator focused on  
**aerothermodynamic fidelity**, **TPS survivability**, and **certification-style robustness analysis**.

Unlike visualization-centric tools, ORCUS is designed as a **failure-aware engineering simulator**, where  
*why and how a vehicle fails* is as important as *whether it survives*.

---

## 🎯 Core Objective (Non-Negotiable)

Maximize **physical realism** for hypersonic re-entry within a **transparent, extensible, research-grade codebase**.

Every phase added to ORCUS exists to:

- Reduce hidden assumptions  
- Expose physical sensitivities  
- Prevent *false survivability*  
- Prepare the model for **CFD / wind-tunnel anchoring**

---

## 🔬 What ORCUS Can Do Today (v0.4-C)

---

## 1️⃣ Flight Dynamics

- 3-DOF point-mass re-entry equations
- Gravity-turn trajectory propagation
- Bank-angle controlled lift modulation
- Numerically stable RK4 integrator
- Explicit vehicle state evolution  
  *(position, velocity, attitude rates)*

---

## 2️⃣ Atmosphere & Gas Properties

- Layered atmosphere model (troposphere + exponential upper layers)
- Altitude-dependent:
  - Temperature
  - Density
  - Speed of sound
- Physically consistent gravity variation with altitude

---

## 3️⃣ Aerothermal Heating (Phase-3)

- Sutton–Graves stagnation-point convective heating
- Enthalpy-limited heating cap
- Mach-gated radiative heating (hypersonic regime)
- Surface re-radiation (Stefan–Boltzmann)
- Net heat flux balance into TPS

---

## 4️⃣ Thermal Protection System (TPS) Modeling

- Multi-parameter TPS material definition
- Time-accurate surface temperature evolution
- Ablation-driven thickness reduction
- Failure modes:
  - Temperature exceedance
  - TPS exhaustion
- Remaining TPS margin tracking

---

## 5️⃣ Guidance & Survivability Logic

- Heat-flux-aware skip guidance
- Dynamic flight-path angle modulation
- Prevents runaway heating while maintaining trajectory realism

---

## 6️⃣ Certification-Style Analysis (Phase-3)

### 🔹 Thermal Margin Quantification (3K)
- Peak temperature ratio (T / Tmax)
- Peak heat-flux tracking
- TPS survivability assessment

### 🔹 Worst-Case Envelope Search (3N)
- Grid sweep over:
  - Entry flight-path angle
  - Bank angle
  - Nose radius
- Identifies *true worst-case* survivability scenarios

### 🔹 Uncertainty Robustness (3P)
- Density perturbations
- Heating uncertainty propagation
- Conservative survivability bounds

### 🔹 Minimum TPS Closure (3Q)
- Searches minimum TPS thickness
- Enforces survival across all certification phases

### 🔹 Monte-Carlo Certification (3W)
- 1000+ randomized re-entry realizations
- Survival statistics
- Failure mode distributions

---

## 7️⃣ CFD-Comparable Flow Physics (Phase-4C)

### 🟦 Phase-4C-1 — Shock Stand-Off & Stagnation Field

- Normal-shock relations along stagnation line
- Isentropic deceleration to stagnation
- Outputs:
  - Shock stand-off distance
  - Stagnation pressure
  - Stagnation temperature
  - Stagnation density

--- Stagnation Flow Field ---
Shock stand-off distance : 0.0319879 m
Stagnation pressure : 320564 Pa
Stagnation temperature : 30495.1 K
Stagnation density : 0.0366272 kg/m^3

---

### 🟦 Phase-4C-2 — Boundary Layer (Integral Solver)

- Momentum-integral boundary-layer formulation
- Laminar → transitional detection via Reynolds criterion
- Hypersonic energy thickness modeling
- Skin-friction and wall shear stress

--- Boundary Layer (Integral Solver) ---
State : Transitional
Momentum thickness : 5.27054e-05 m
Energy thickness : 5.88046e-05 m
Skin friction coeff Cf : 0.00131764
Wall shear stress : 1468.11 Pa


---

### 🟦 Phase-4C-3 — Wall Heat Flux from BL Theory

- Stanton number formulation
- Laminar vs transitional amplification
- Wall heat flux computed from:
  - Edge density
  - Edge velocity
  - Stagnation temperature
  - Wall temperature

--- Wall Heat Flux (BL Theory) ---
Stanton number : 0.00141
Wall heat flux q_w : 1.12e+07 W/m^2


---

## 📊 Sample Monte-Carlo Certification Output

--- Monte-Carlo Results ---
Total runs : 1000
Survived : 238
Temp failures : 12
Ablation failures : 750
Survival rate : 23.8 %
Worst T/Tmax : 1.29602
Worst remaining TPS : 0

---

## 📁 Repository Structure

ORCUS/
├── include/ # Public headers
│ ├── orcus_core.h
│ ├── orcus_physics.h
│ ├── orcus_heat.h
│ ├── orcus_flowfield.h
│ ├── orcus_boundary_layer.h
│ ├── orcus_bl_heating.h
│ └── ...
│
├── src/ # Implementation
│ ├── orcus_core.cpp
│ ├── orcus_dynamics.cpp
│ ├── orcus_heat.cpp
│ ├── orcus_flowfield.cpp
│ ├── orcus_boundary_layer.cpp
│ ├── orcus_bl_heating.cpp
│ └── ...
│
├── docs/ # Technical documentation (planned)
├── data/ # Test cases / reference data
├── python/ # Post-processing & plotting (planned)
└── README.md

---

## 🧭 Roadmap (Planned & In-Progress)

### 🔜 Phase-4C-4 — Diffusive Navier–Stokes Heat Flux
- Parabolized NS energy equation
- Diffusion-dominated wall heat transfer
- Bridges BL theory → CFD fidelity

### 🔜 Phase-4D — Trajectory-Coupled Aerothermodynamics
- Heating feedback into dynamics
- Local flow-field evolution along trajectory

### 🔜 Phase-5 — CFD / Wind-Tunnel Anchoring
- Calibration against:
  - CFD datasets
  - Experimental stagnation heating data
- Error envelopes & correction models

---

## ⚠️ Design Philosophy

- **Additive development only**
- No silent physics overrides
- No hidden “safety factors”
- Every failure is explicit and traceable

ORCUS is designed to **fail honestly** — because honest failure is how aerospace systems survive.

---

## 📜 License

MIT License — free for research, academic use, and extension  
(see `LICENSE.md`)

---

> *ORCUS is not built to look correct — it is built to be wrong in the same way reality is wrong.*