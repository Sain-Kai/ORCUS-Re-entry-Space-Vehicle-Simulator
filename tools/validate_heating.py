#!/usr/bin/env python3
"""
validate_heating.py - simple sanity checks comparing Sutton & Fay values in a CSV
Usage:
    python tools/validate_heating.py ../../data/orcus_trace.csv
"""

import sys, pandas as pd
import numpy as np

def main():
    if len(sys.argv) < 2:
        print("Usage: validate_heating.py <orcus_csv>")
        return
    df = pd.read_csv(sys.argv[1])
    print("Rows:", len(df))
    if "qlam" in df.columns and "q_fay" in df.columns:
        ratio = (df["q_fay"] + 1e-9) / (df["qlam"] + 1e-9)
        print("q_fay / q_lam  stats: min %.3f median %.3f max %.3f" % (ratio.min(), ratio.median(), ratio.max()))
    if "q_total" in df.columns:
        print("q_total stats (W/m2): min %.3e median %.3e max %.3e" % (df["q_total"].min(), df["q_total"].median(), df["q_total"].max()))
    # find time of peak heating
    if "q_total" in df.columns:
        idx = df["q_total"].idxmax()
        print("Peak q_total at row", idx, "t=", df.loc[idx, "t"], "s h=", df.loc[idx, "h"], "m q=", df.loc[idx,"q_total"])

if __name__ == "__main__":
    main()
