#!/usr/bin/env python3
"""
batch_run.py
Run a sweep described in configs/sweep_example.yaml.
Saves CSV outputs into runs_dir with descriptive filenames.

Usage:
    python tools/batch_run.py configs/sweep_example.yaml
"""

import sys, os, subprocess, yaml, itertools, shutil
from pathlib import Path

def load_yaml(p):
    with open(p, 'r') as f:
        return yaml.safe_load(f)

def main():
    if len(sys.argv) < 2:
        print("Usage: batch_run.py <sweep_yaml>")
        return
    sweep_cfg = load_yaml(sys.argv[1])
    base_cfg_file = sweep_cfg.get("base_config")
    runs_dir = Path(sweep_cfg.get("runs_dir", "data/batch_runs")).resolve()
    runs_dir.mkdir(parents=True, exist_ok=True)

    base_cfg = load_yaml(Path("configs") / base_cfg_file)
    sweep = sweep_cfg.get("sweep", [])
    # build param grid
    param_names = [p["param"] for p in sweep]
    param_lists = [p["values"] for p in sweep]
    combos = list(itertools.product(*param_lists))
    print(f"Running {len(combos)} runs...")

    orcus_exe = Path("x64/Debug/ORCUS.exe").resolve()
    if not orcus_exe.exists():
        print("ERROR: ORCUS.exe not found at", orcus_exe)
        return

    for combo in combos:
        # create per-run config by copying base and replacing params
        run_cfg = dict(base_cfg)
        run_name_parts = []
        for name, val in zip(param_names, combo):
            # support vehicle params Cl and Rn at top-level under vehicle
            if "vehicle" not in run_cfg: run_cfg["vehicle"] = {}
            run_cfg["vehicle"][name] = float(val)
            run_name_parts.append(f"{name}{val}")
        run_name = "_".join(run_name_parts)
        out_csv = runs_dir / f"run_{run_name}.csv"
        # build CLI args for ORCUS.exe from run_cfg
        args = [str(orcus_exe)]
        # heat model
        heat = run_cfg.get("physics", {}).get("heat_model", "sutton")
        if heat == "fay":
            args += ["--heat", "fay"]
        # Re thresholds
        Re_lam = run_cfg.get("physics", {}).get("Re_lam", "")
        Re_crit = run_cfg.get("physics", {}).get("Re_crit", "")
        if Re_lam:
            args += ["--Re_lam", str(Re_lam)]
        if Re_crit:
            args += ["--Re_crit", str(Re_crit)]
        # lift
        Cl = run_cfg.get("vehicle", {}).get("Cl", 0.0)
        args += ["--Cl", str(Cl)]
        # Rn (we pass via --Rn by updating ORCUS CLI? We don't have --Rn currently; we'll pass by env var)
        Rn = run_cfg.get("vehicle", {}).get("Rn", None)
        if Rn is not None:
            os.environ["ORCUS_Rn_override"] = str(Rn)
        # control
        if run_cfg.get("control", {}).get("auto_bank", False):
            args += ["--auto_bank"]
            q_target = run_cfg.get("control", {}).get("q_target", None)
            if q_target:
                args += ["--q_target", str(q_target)]
            max_bank = run_cfg.get("control", {}).get("max_bank", None)
            if max_bank:
                args += ["--max_bank", str(max_bank)]
        # output path
        args += ["--out", str(out_csv)]
        print("Running:", " ".join(args))
        try:
            subprocess.run(args, check=True, env=os.environ.copy())
        except subprocess.CalledProcessError as e:
            print("Run failed:", e)
        # cleanup ORCUS_Rn_override
        if "ORCUS_Rn_override" in os.environ:
            del os.environ["ORCUS_Rn_override"]

    print("Batch complete. Outputs in", runs_dir)

if __name__ == "__main__":
    main()
