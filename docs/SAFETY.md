# Data-recovery safety

For genuinely failing hardware, repeated reads can worsen the situation. RESURGE v0.1 is designed around **images and clones**, not live repair. Work from a clone when possible, preserve the original, never mount damaged media read-write, and do not write recovered data back to the source device.

This project cannot recreate arbitrary information that has been physically destroyed or securely overwritten when no redundancy/constraint information survives. It intentionally marks such regions `LOST` or `AMBIGUOUS` rather than inventing bytes.
