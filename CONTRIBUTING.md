# Contributing

RESURGE is a research project. Contributions should preserve three rules:

1. never silently turn inference into fact;
2. never mutate source evidence by default;
3. every recovery claim should be reproducible on a ground-truth fixture when practical.

Useful contribution areas include filesystem parsers, journals/WAL, integrity constraints, clone fusion, uncertainty calibration, corruption generators, visualization, fuzzing and formalization.

Pull requests should include tests and describe whether new evidence is independent of existing evidence or derived from the same source.
