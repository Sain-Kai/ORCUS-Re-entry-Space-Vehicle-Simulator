# 📦 ORCUS — Changelog

All notable changes to ORCUS are documented here.

---

## [v0.3] — Thermal Closure & TPS Failure (Current)

### Added
- Thermal Protection System (TPS) abstraction
- Sutton–Graves stagnation heating model
- Parametric Fay–Riddell baseline comparison
- Heat flux accumulation over time
- Thermal closure execution phase
- TPS failure detection and mission abort logic

### Changed
- Centralized physical constants (`constants.h`)
- Refactored heating computation into dedicated module
- Corrected function signatures across physics layers
- Clear separation between aerodynamics, heating, and TPS response

### Fixed
- Inconsistent heat flux variable usage
- Function argument mismatches
- Redundant constant redeclarations

---

## [v0.2] — Aerodynamics & Heating Foundation

### Added
- Mach-dependent drag coefficient (piecewise model)
- Exponential atmospheric density model
- Dynamic pressure computation
- Initial hypersonic heating logic

### Changed
- Modularized physics into atmosphere / aero / thermal
- Improved RK4 numerical stability

---

## [v0.1] — Core Re-Entry Skeleton

### Added
- 3-DOF point-mass re-entry equations
- RK4 numerical integrator
- Initial trajectory propagation framework
- Basic project structure

---

## Planned
- CFD / wind-tunnel validated tables
- Reynolds-number-based transition logic
- Guidance & control modules
- Visualization and telemetry export
