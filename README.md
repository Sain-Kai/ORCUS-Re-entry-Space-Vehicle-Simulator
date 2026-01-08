\# ORCUS — Hypersonic Re-Entry Vehicle Simulation Framework



ORCUS is a \*\*modular, physics-driven hypersonic atmospheric re-entry vehicle simulator\*\* focused on \*\*aerothermodynamic fidelity\*\*, \*\*Thermal Protection System (TPS) survivability\*\*, and \*\*certification-style robustness analysis\*\*.



Unlike visualization-centric trajectory tools, ORCUS is designed as a \*\*failure-aware engineering simulator\*\*, where \*why\* and \*how\* a vehicle fails is treated with the same importance as \*whether\* it survives.



The codebase prioritizes \*\*transparent physics\*\*, \*\*explicit assumptions\*\*, and \*\*incremental validation readiness\*\* for CFD and wind-tunnel comparison.



---



\## Core Objective (Non-Negotiable)



> \*\*Maximize physical realism for hypersonic re-entry within a transparent, extensible, research-grade codebase.\*\*



Every phase implemented in ORCUS exists to:



\- Reduce hidden assumptions  

\- Expose physical sensitivities  

\- Prevent \*false survivability\* conclusions  

\- Prepare the solver for \*\*CFD / wind-tunnel anchoring\*\*



---



\## Design Philosophy



\- Physics before performance  

\- Explicit failure modes  

\- Additive development only (no silent refactors)  

\- Certification traceability over convenience  

\- Reduced-order CFD surrogate, not a black-box predictor  



ORCUS intentionally avoids monolithic solvers and instead builds fidelity \*\*layer-by-layer\*\*, mirroring real aerospace analysis workflows.



---



\## What ORCUS Can Do Today (v0.4-C)



\### 1. Flight Dynamics



\- 3-DOF point-mass re-entry equations  

\- Gravity-turn trajectory propagation  

\- Bank-angle–controlled lift modulation  

\- Numerically stable \*\*RK4\*\* time integration  

\- Explicit vehicle state evolution:

&nbsp; - Position

&nbsp; - Velocity

&nbsp; - Attitude rates



---



\### 2. Atmosphere \& Gas Properties



\- Altitude-dependent temperature model  

\- Density stratification with exponential upper atmosphere  

\- Variable speed of sound  

\- Consistent thermodynamic closure for Mach computation  



---



\### 3. Aerodynamics



\- Mach-dependent lift and drag modeling  

\- Static stability derivatives  

\- Bank-angle coupling into trajectory evolution  

\- Hypersonic-appropriate coefficient scaling  



---



\### 4. Aerothermal Heating (Phase-3)



\- Fay–Riddell–based stagnation convective heating  

\- Enthalpy-limited heat flux formulation  

\- Hypersonic radiative heating activation  

\- Surface re-radiation via Stefan–Boltzmann law  

\- Net heat flux coupling into TPS response  



---



\### 5. Thermal Protection System (TPS)



\- Transient surface and bulk temperature evolution  

\- Thickness depletion via ablation  

\- Explicit TPS failure modes:

&nbsp; - Thermal exceedance  

&nbsp; - Material exhaustion  

\- Remaining TPS margin tracking  



---



\### 6. Guidance \& Survivability Logic



\- Heat-flux-aware skip guidance  

\- Trajectory modulation to enforce thermal constraints  

\- Failure-aware state termination  



---



\### 7. Robustness \& Certification Analysis



\- Worst-case envelope search (Phase-3N)  

\- Uncertainty robustness analysis (Phase-3P)  

\- Minimum TPS thickness closure (Phase-3Q)  

\- Monte-Carlo certification runs (Phase-3W)  



These phases explicitly answer:



\- Where does the vehicle fail?  

\- How sensitive is survival to uncertainty?  

\- Which margins are illusory?  



---



\## Phase-4C — CFD-Comparable Aerothermodynamics



Phase-4C introduces \*\*post-processing physics layers\*\* to bridge ORCUS with high-fidelity CFD solvers.



\### Phase-4C-1 — Shock Stand-Off \& Stagnation Field



\- Normal shock relations along stagnation streamline  

\- Shock stand-off distance estimation  

\- Stagnation pressure, temperature, and density extraction  



\*\*Purpose:\*\* Establish CFD-comparable boundary conditions at the vehicle nose.



---



\### Phase-4C-2 — Boundary-Layer Integral Solver



\- Reynolds-number–based laminar → transitional logic  

\- Momentum thickness (θ)  

\- Energy thickness (δₑ)  

\- Skin-friction coefficient (Cᶠ)  

\- Wall shear stress (τw)  



\*\*Purpose:\*\* Replace empirical heating correlations with flow-resolved surface physics.



---



\### Phase-4C-3 — Wall Heat Flux from Boundary-Layer Theory



\- Stanton number via Reynolds analogy  

\- Laminar and transitional correction logic  

\- Convective wall heat flux evaluation  



\*\*Purpose:\*\* Derive heat flux from boundary-layer physics rather than stagnation-only models.



---



\### Phase-4C-4 — Parabolized Navier–Stokes Energy Equation



\- Wall-normal temperature gradient estimation  

\- Fourier-law diffusive heat flux  

\- Explicit separation of convective and diffusive heating  



\*\*Purpose:\*\* Introduce Navier–Stokes energy transport effects without full CFD cost.



---



\## Example Results



\### Stagnation Flow Field



Shock stand-off distance : 0.03199 m

Stagnation pressure : 320564 Pa

Stagnation temperature : 30495 K

Stagnation density : 0.0366 kg/m³



shell

Copy code



\### Boundary Layer (Integral Solver)



State : Transitional

Momentum thickness : 5.27e-05 m

Energy thickness : 5.88e-05 m

Skin friction coeff Cf : 0.001317

Wall shear stress : 1468 Pa



shell

Copy code



\### Monte-Carlo Certification Summary



Total runs : 1000

Survived : 238

Thermal failures : 12

Ablation failures : 750

Survival rate : 23.8 %

Worst T/Tmax : 1.296

Worst remaining TPS : 0



yaml

Copy code



---



\## Directory Structure



ORCUS/

├── include/ # Public headers

│ ├── orcus\_core.h

│ ├── orcus\_heat.h

│ ├── orcus\_flowfield.h

│ ├── orcus\_boundary\_layer.h

│ ├── orcus\_bl\_heating.h

│ ├── orcus\_pns\_energy.h

│ └── ...

│

├── src/ # Implementation files

│ ├── orcus\_core.cpp

│ ├── orcus\_dynamics.cpp

│ ├── orcus\_heat.cpp

│ ├── orcus\_flowfield.cpp

│ ├── orcus\_boundary\_layer.cpp

│ ├── orcus\_bl\_heating.cpp

│ ├── orcus\_pns\_energy.cpp

│ └── ...

│

├── data/ # Input profiles / reference datasets

├── docs/ # Design notes and derivations

├── python/ # Post-processing \& analysis scripts

├── sim/ # Simulation outputs

└── main.cpp



yaml

Copy code



---



\## Validation Philosophy



ORCUS is \*\*not tuned to match CFD by default\*\*.



Instead:



\- Each physics layer can be anchored independently  

\- Differences vs CFD are diagnosable  

\- Agreement (or disagreement) has physical meaning  



This makes ORCUS suitable for:



\- Early-phase design trade studies  

\- TPS survivability screening  

\- CFD pre-conditioning  

\- Wind-tunnel test planning  



---



\## Roadmap



\### Near-Term

\- Phase-4C-5: Shock-layer finite thickness correction  

\- Phase-4C-6: Equilibrium / frozen chemistry coupling  

\- CSV export for CFD comparison  



\### Mid-Term

\- Phase-4D: Trajectory-coupled aerothermal feedback  

\- Surface-resolved heat flux mapping  

\- Multi-material TPS modeling  



\### Long-Term

\- Reduced-order surrogate generation from CFD  

\- Flight data anchoring  

\- Certification-oriented margin closure workflows  



---



\## License



MIT License — see `LICENSE.md`

