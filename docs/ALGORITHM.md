# MERA-ΩX PRIME

MERA-ΩX PRIME is the experimental reconstruction algorithm inside RESURGE BLACKLAB.

## Core idea

A missing region is not filled by a single heuristic. The engine represents it as an unknown variable in an evidence hypergraph, generates competing candidates, and attempts to **eliminate candidates by contradiction**. A candidate may be promoted to `PROVEN` only when the declared trusted constraint set leaves one surviving solution. Heuristic preference alone can produce at most `INFERRED`.

## Evidence classes

Physical repeated reads, filesystem structures, journals/WAL, checksums, file-format invariants, replicas/parity, temporal relationships, neighboring structures, and application-level structure can all contribute. Each evidence item carries reliability and independence factors so correlated evidence is not treated as independent confirmation.

## Counterfactual evaluation

For candidate `H`, the conceptual evaluator asks: if `H` occupied this region, what other statements about the disk would have to be true? Violations of reliable invariants eliminate `H`. This is implemented as local constraints in v0.1; graph-wide propagation is a roadmap item.

## Recovery entropy

The solver normalizes the posterior mass of surviving candidates into an entropy score from 0 (one effective solution) to 1 (maximal uncertainty among survivors). Entropy is used for presentation and active-acquisition planning, never as proof by itself.

## Active acquisition

Read targets are ranked by an experimental value-of-information score:

`priority = information_gain * (0.5 + impact) * (0.5 + uniqueness) / max(media_stress, 0.02)`

A production implementation would estimate media stress from the acquisition layer and should default to conservative read-only behavior.

## Truth levels

- `PHYSICAL`: bytes read from source/clone and accepted by acquisition policy.
- `PROVEN`: unique candidate under the declared trusted constraints.
- `CORROBORATED`: extremely strong independent support, but not a formal uniqueness claim.
- `INFERRED`: best-supported candidate while alternatives remain possible.
- `AMBIGUOUS`: multiple credible candidates survive.
- `LOST`: insufficient evidence.

## False certainty is the primary failure

The benchmark suite explicitly measures wrong bytes that were presented as reconstructed. The design prefers `UNKNOWN` over a plausible but unsupported guess.
