# Architecture

```text
read-only images / clone sets
          |
          v
   acquisition evidence
          |
          +----> C++23 MERA engine ----> region decisions
          |             |                    |
          |             +--> entropy --------+
          |             +--> clone fusion ---+
          |             +--> read priorities-+
          |                                  |
          +----------------------------------v
                                   immutable overlay plan
                                           |
                    +----------------------+----------------+
                    v                      v                v
                 Go control          TypeScript Studio   certificate
                    ^
                    |
            Java evidence plugins

Research paths (source included, not verified in default environment):
CUDA candidate pruning · Zig micro-adapters · Julia experiments · Lean proof model · SystemVerilog preprocessing
```

v0.1 intentionally does **not** write to physical disks. Acquisition is modeled as read targets and image/clone evidence. Future raw-device readers must be isolated, read-only by default, and tested against virtual devices before any hardware use.
