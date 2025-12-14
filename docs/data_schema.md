# ORCUS CSV Schema (columns & meanings)

t            : time (s)
h            : altitude (m)
v            : velocity (m/s)
gamma        : flight-path angle (rad)
x            : downrange distance (m)
Mach         : Mach number (unitless)
rho          : freestream density (kg/m^3)
Tinf         : freestream temperature (K)
qlam         : laminar heat flux (arb units / W/m^2)
qturb        : base turbulent heat metric (unblended)
q_fay        : Fay-Riddell approximate heating (unblended)
q_total      : total blended heating (W/m^2)
transition_f : transition blending fraction [0..1]
Cl           : lift coefficient used
L            : lift magnitude (N)
L_vert       : vertical component of lift (N) after bank
bank_cmd     : commanded bank angle (deg)
cum_heat     : integrated cumulative heat (W*s/m^2)
dt           : timestep used (s)
accel_g      : acceleration in g (dimensionless)
