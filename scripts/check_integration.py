#!/usr/bin/env python3
import json, pathlib, subprocess, sys
root=pathlib.Path(__file__).resolve().parents[1]
binpath=root/"engine-cpp/build/resurge-engine"
p=subprocess.run([str(binpath)],capture_output=True,text=True,check=True)
d=json.loads(p.stdout.strip().splitlines()[0])
assert d["level"] == "PROVEN", d
assert d["candidate"] == "candidate-A", d
assert d["normalized_entropy"] == 0.0, d
assert "e-crc" in d["proof_evidence"], d
print("cross-language integration passed: C++ decision parsed/validated by Python")
