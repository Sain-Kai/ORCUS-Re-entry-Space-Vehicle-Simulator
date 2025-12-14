#!/usr/bin/env python3
"""
plot_orcus.py - quick visualizer for ORCUS CSV output
Usage:
    python tools/plot_orcus.py ../../data/orcus_trace.csv
"""

import sys, pandas as pd
import matplotlib.pyplot as plt

def main():
    if len(sys.argv) < 2:
        print("Usage: plot_orcus.py <csv>")
        return
    csv = sys.argv[1]
    df = pd.read_csv(csv)
    t = df["t"]
    plt.figure(); plt.plot(t, df["h"]); plt.title("Altitude (m) vs time"); plt.xlabel("t (s)"); plt.ylabel("h (m)")
    plt.grid(); plt.show()
    plt.figure(); plt.plot(t, df["v"]); plt.title("Velocity (m/s) vs time"); plt.xlabel("t (s)"); plt.grid(); plt.show()
    if "Mach" in df.columns:
        plt.figure(); plt.plot(t, df["Mach"]); plt.title("Mach vs time"); plt.xlabel("t (s)"); plt.grid(); plt.show()
    if "q_total" in df.columns:
        plt.figure(); plt.plot(t, df["q_total"]); plt.title("Total heat flux vs time"); plt.xlabel("t (s)"); plt.yscale("log"); plt.grid(); plt.show()

if __name__ == "__main__":
    main()
