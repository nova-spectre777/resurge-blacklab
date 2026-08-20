from __future__ import annotations
from dataclasses import dataclass, asdict
import hashlib, json, math, random, zlib
from .corruption import corrupt

MAGIC = b"RSG1"

@dataclass
class BlockTruth:
    index: int
    payload: bytes
    crc: int

@dataclass
class BenchmarkResult:
    seed: int
    corruption_rate: float
    total_payload_bytes: int
    damaged_payload_bytes: int
    exact_recovered_bytes: int
    wrong_reconstructed_bytes: int
    unknown_bytes: int
    exact_recovery_rate: float
    false_certainty_rate: float


def build_fixture(blocks: int = 64, payload_size: int = 128, seed: int = 7) -> tuple[bytes, list[BlockTruth]]:
    """Build a tiny research container with two replicas + CRC metadata per block."""
    rng = random.Random(seed)
    truths=[]; image=bytearray(MAGIC)
    image += blocks.to_bytes(4,"little") + payload_size.to_bytes(4,"little")
    for i in range(blocks):
        payload = bytes(rng.randrange(0,256) for _ in range(payload_size))
        crc = zlib.crc32(payload)
        truths.append(BlockTruth(i,payload,crc))
        image += i.to_bytes(4,"little") + crc.to_bytes(4,"little") + payload + payload
    return bytes(image), truths


def recover_fixture(damaged: bytes, truths: list[BlockTruth], payload_size: int = 128) -> tuple[bytes, list[str]]:
    """Recover using replica agreement + CRC. Ambiguous bytes remain zero and are labeled UNKNOWN."""
    header=12; stride=8+payload_size*2
    recovered=bytearray(); labels=[]
    for t in truths:
        base=header+t.index*stride
        crc=int.from_bytes(damaged[base+4:base+8],"little")
        a=damaged[base+8:base+8+payload_size]
        b=damaged[base+8+payload_size:base+8+2*payload_size]
        candidates=[]
        if any(a): candidates.append((a,"REPLICA_A"))
        if any(b): candidates.append((b,"REPLICA_B"))
        valid=[(p,s) for p,s in candidates if zlib.crc32(p)==crc]
        if len(valid)==1:
            recovered += valid[0][0]; labels += ["PROVEN"]*payload_size
        elif len(valid)>1 and all(p==valid[0][0] for p,_ in valid):
            recovered += valid[0][0]; labels += ["CORROBORATED"]*payload_size
        else:
            recovered += b"\x00"*payload_size; labels += ["UNKNOWN"]*payload_size
    return bytes(recovered), labels


def benchmark(rate: float=.05, seed: int=11, cluster: int=16) -> BenchmarkResult:
    image, truths=build_fixture()
    damaged, _=corrupt(image,rate=rate,seed=seed,cluster=cluster)
    out, labels=recover_fixture(damaged,truths)
    truth=b"".join(t.payload for t in truths)
    exact=wrong=unknown=0
    damaged_bytes=0
    header=12; stride=8+128*2
    for t in truths:
        base=header+t.index*stride+8
        damaged_bytes += sum(x!=y for x,y in zip(damaged[base:base+128],t.payload))
    for got,want,label in zip(out,truth,labels):
        if label=="UNKNOWN": unknown+=1
        elif got==want: exact+=1
        else: wrong+=1
    total=len(truth)
    return BenchmarkResult(seed,rate,total,damaged_bytes,exact,wrong,unknown,exact/total,wrong/max(1,exact+wrong))


def report(result: BenchmarkResult) -> str:
    d=asdict(result); d["algorithm"]="MERA-OmegaX-Prime/0.1-lab"; d["result_digest"]=hashlib.sha256(json.dumps(d,sort_keys=True).encode()).hexdigest(); return json.dumps(d,indent=2,sort_keys=True)
