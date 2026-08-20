# Threat model

RESURGE processes untrusted disk images and filesystem/file-format structures. Parsers must assume malformed lengths, cyclic references, giant allocations, decompression bombs, crafted metadata and adversarial candidate sets.

Security boundaries:
- never execute recovered binaries, scripts, macros, filesystem hooks or autorun content;
- image parsers are read-only;
- output recovery overlays are separate from source images;
- all externally supplied lengths/counts require bounds;
- plugin evidence cannot directly upgrade a region to `PROVEN`;
- proof/certificate claims must include the algorithm and evidence version;
- encryption bypass, credential cracking and access-control circumvention are outside the project scope.
