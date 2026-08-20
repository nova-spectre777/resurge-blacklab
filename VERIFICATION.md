# RESURGE BLACKLAB v0.1 verification

Verified locally on 2026-08-19 with the source in this package.

## Default verified stack

- C++23 / CMake: `ctest` — 1/1 passed
- Go 1.23: `go test ./...` — API and planner packages passed
- Python 3.13: 4/4 unit tests passed
- TypeScript 5.8 / Node 22: 3/3 tests passed under strict compilation
- Java 21: plugin SDK compilation and executable test passed
- Cross-language: C++ solver JSON was parsed and semantically validated by Python

## Synthetic corruption benchmark fixture

Configuration: 3% deterministic corruption, seed 17, cluster size 8.

- ground-truth payload bytes: 8192
- exact recovered bytes: 6528
- wrong reconstructed bytes: 0
- unknown bytes: 1664
- exact recovery rate: 0.796875
- false certainty rate: 0.0

These numbers apply only to the included synthetic redundant container and are not real-drive recovery claims.

## Research source tracks not verified in this environment

CUDA (`nvcc`), Zig, Julia, Lean 4 and Icarus/SystemVerilog toolchains were not installed. Source interfaces are included but are not labelled tested.
