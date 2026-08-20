from __future__ import annotations
from dataclasses import dataclass
import random

@dataclass(frozen=True)
class Damage:
    offset: int
    length: int
    mode: str


def corrupt(data: bytes, *, rate: float, seed: int = 1, cluster: int = 1) -> tuple[bytes, list[Damage]]:
    """Deterministically replace selected bytes with zero for benchmark purposes.

    This operates on synthetic/image copies only. It never writes to a source device.
    """
    if not 0 <= rate <= 1:
        raise ValueError("rate must be in [0,1]")
    if cluster < 1:
        raise ValueError("cluster must be >= 1")
    rng = random.Random(seed)
    out = bytearray(data)
    target = int(len(data) * rate)
    starts: set[int] = set()
    while sum(min(cluster, len(data)-s) for s in starts) < target and len(starts) < len(data):
        starts.add(rng.randrange(0, max(1, len(data))))
    damage: list[Damage] = []
    for s in sorted(starts):
        n = min(cluster, len(data)-s)
        if n <= 0: continue
        out[s:s+n] = b"\x00" * n
        damage.append(Damage(s,n,"erasure"))
    return bytes(out), damage
