<div align="center">

# RESURGE BLACKLAB
### MERA-ΩX PRIME · Proof-Carrying Active Data Reconstruction

**Experimental, read-only-first research platform for reconstructing damaged storage images from multiple independent evidence layers.**

</div>

RESURGE BLACKLAB explores a recovery model that treats missing regions as **unknown variables**, not blank space to guess. Its MERA-ΩX PRIME engine creates competing reconstruction hypotheses, eliminates candidates that contradict trusted constraints, estimates residual uncertainty, records provenance, and can tell the acquisition layer which unreadable region would be most valuable to retry next.

> **Research status:** v0.1 foundation. This is not a commercial forensic product, not a certified evidence-processing suite, and not a claim that arbitrary overwritten/destroyed bytes can be recovered. The architecture is an experimental combination; the project does not claim worldwide patent novelty.

## What is different

```text
                         damaged image / clone set
                                  |
                                  v
                         physical observations
                                  |
      +---------------------------+---------------------------+
      |                           |                           |
      v                           v                           v
 filesystem/journal         format/application          checksums/replicas
      |                           |                           |
      +---------------------------+---------------------------+
                                  v
                         EVIDENCE HYPERGRAPH
                                  |
                         unknown region X
                                  |
                    +-------------+-------------+
                    v             v             v
                   H1            H2            H3
                    |             |             |
                    +------ counterfactual -----+
                              evaluation
                                  |
                         contradiction pruning
                                  |
                         entropy / uniqueness
                                  |
             +--------------------+--------------------+
             v                    v                    v
           PROVEN              INFERRED             LOST
             |
             v
      immutable recovery overlay + certificate
```

The central rule is simple: **a plausible byte is not an original byte**. RESURGE tracks how every region was obtained.

## Truth levels

| Level | Meaning |
|---|---|
| `PHYSICAL` | accepted physical/clone read |
| `PROVEN` | one candidate survives the declared trusted constraints |
| `CORROBORATED` | extremely strong independent support, without a formal uniqueness claim |
| `INFERRED` | best-supported candidate while alternatives may remain |
| `AMBIGUOUS` | multiple credible candidates survive |
| `LOST` | insufficient surviving information |

## v0.1 working components

| Component | Language | Status | Purpose |
|---|---|---|---|
| `engine-cpp/` | **C++23** | verified | counterfactual solver, entropy, active-acquisition ranking, multi-clone fusion |
| `control-go/` | **Go** | verified | recovery-control API, read-target planning, SHA-256 recovery certificates |
| `lab-python/` | **Python** | verified | deterministic corruption arena, ground-truth benchmark, immutable overlays |
| `studio-ts/` | **TypeScript** | verified | recovery provenance visualization primitives |
| `plugin-java/` | **Java 21** | verified | file/application evidence plugin SDK |

### Research source tracks

The repository also includes source-level research interfaces for:

- **CUDA C++** — parallel cheap-constraint hypothesis pruning
- **Zig** — tiny portable evidence adapters
- **Julia** — entropy/calibration experiments
- **Lean 4** — formal model of a unique-recovery claim
- **SystemVerilog** — optional confidence-preprocessing primitive

Those five toolchains are **not installed in the build environment used for this release**, so they are intentionally not labelled verified.

## MERA-ΩX PRIME concepts implemented now

### Contradiction-first candidate elimination
A strong failed invariant can eliminate a candidate instead of merely reducing an opaque score.

### Recovery entropy
Surviving candidate mass is converted into normalized uncertainty. Entropy describes ambiguity; it does not create proof.

### Active acquisition feedback
Damaged regions can be ranked by expected information gain, dependency impact, evidence uniqueness, and estimated media stress.

### Multi-clone fusion
Several imperfect images can contribute weighted byte observations while preserving ambiguity instead of blind majority voting.

### Immutable recovery overlays
Recovered bytes live in a separate overlay bound to the source image digest. The source image is never modified.

### Recovery certificates
The Go control layer creates a digest-bound certificate carrying algorithm version, image/overlay digests, and region provenance.

## Reproducible corruption arena

The Python laboratory builds known-good synthetic storage containers, deliberately corrupts copies, reconstructs from remaining redundancy/CRC evidence, and compares output against the hidden ground truth.

Example from the v0.1 fixture:

```text
corruption rate:          3%
exact recovered bytes:    6528 / 8192
wrong reconstructed:      0
unknown bytes:            1664
false certainty rate:     0.0
```

This is a synthetic benchmark, **not a claim about real hard-drive recovery rates**.

## Quick verification

Requirements for the default verified stack:

- CMake + C++23 compiler
- Go 1.23+
- Python 3.11+
- Node.js + TypeScript compiler
- Java 21+

Run:

```bash
./scripts/check_all.sh
```

Run the two demos:

```bash
cmake -S engine-cpp -B engine-cpp/build
cmake --build engine-cpp/build
./engine-cpp/build/resurge-engine

PYTHONPATH=lab-python python3 -m resurge_lab.cli benchmark --rate .03 --seed 17 --cluster 8
```

## Example solver result

```json
{
  "offset": 4096,
  "length": 7,
  "level": "PROVEN",
  "confidence": 1.0,
  "normalized_entropy": 0.0,
  "candidate": "candidate-A",
  "proof_evidence": ["e-crc", "e-journal", "e-neighbor"]
}
```

The competing fixture candidate is eliminated because it violates a high-reliability structured checksum constraint.

## Safety model

RESURGE v0.1 operates on **images, clones, synthetic fixtures, and read plans**. It has no physical-disk write path.

For genuinely failing media:

1. preserve the original;
2. acquire a clone/image with established forensic/recovery tooling when possible;
3. do analysis on the clone;
4. never write recovered output back onto the source;
5. treat repeated reads as a media-risk decision, not a free operation.

Encryption bypass, password cracking, access-control circumvention, and claims of recovering securely overwritten information are outside this project.

## Repository map

```text
engine-cpp/          C++23 MERA core
control-go/          control/certificate service
lab-python/          corruption arena + overlays
studio-ts/           visualization foundation
plugin-java/         evidence plugin SDK
contracts/           JSON interchange contracts
cuda-kernels/        experimental CUDA research
zig-microkernel/     experimental Zig adapter
julia-lab/           experimental Julia research
lean-proofs/         experimental Lean model
fpga-systemverilog/  experimental HDL preprocessing
examples/            fixtures
docs/                algorithm, architecture, roadmap, safety, threat model
scripts/             verification/integration
```

## Next research milestones

- ext4 and NTFS evidence adapters
- ddrescue map importer
- persistent evidence hypergraph
- temporal journal/WAL reconstruction
- graph-wide constraint propagation
- virtual block-device overlay
- calibrated confidence on real public disk-image corpora
- optional GPU hypothesis-pruning benchmark
- formally checkable uniqueness certificate subset

See [`docs/ROADMAP.md`](docs/ROADMAP.md).

## Responsible contribution

The most valuable contribution is not a clever guess. It is a **new independently testable constraint**, a safer acquisition method, a reproducible corruption case, or a proof that the engine should refuse reconstruction.

See [`CONTRIBUTING.md`](CONTRIBUTING.md).
