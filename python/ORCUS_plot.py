import pandas as pd
import matplotlib.pyplot as plt

# Load ORCUS CSV
df = pd.read_csv("orcus_trace.csv")

# Time
t = df["t(s)"]

# 1) Altitude vs Time
plt.figure(figsize=(10,6))
plt.plot(t, df["h(m)"])
plt.title("Altitude vs Time")
plt.xlabel("Time (s)")
plt.ylabel("Altitude (m)")
plt.grid()
plt.show()

# 2) Velocity vs Time
plt.figure(figsize=(10,6))
plt.plot(t, df["v(m/s)"])
plt.title("Velocity vs Time")
plt.xlabel("Time (s)")
plt.ylabel("Velocity (m/s)")
plt.grid()
plt.show()

# 3) Mach Number vs Time
plt.figure(figsize=(10,6))
plt.plot(t, df["Mach"])
plt.title("Mach Number vs Time")
plt.xlabel("Time (s)")
plt.ylabel("Mach")
plt.grid()
plt.show()

# 4) Heat Flux vs Time
plt.figure(figsize=(10,6))
plt.plot(t, df["q_lam(arbitrary)"])
plt.title("Laminar Heat Flux vs Time")
plt.xlabel("Time (s)")
plt.ylabel("Heat Flux (W/m2 - scaled)")
plt.grid()
plt.show()

# 5) g-load vs Time
plt.figure(figsize=(10,6))
plt.plot(t, df["accel_g"])
plt.title("Acceleration (g-load) vs Time")
plt.xlabel("Time (s)")
plt.ylabel("g-load")
plt.grid()
plt.show()
