from __future__ import annotations
from dataclasses import dataclass, asdict
import hashlib, json

@dataclass(frozen=True)
class Patch:
    offset: int
    data_hex: str
    level: str
    evidence: tuple[str, ...] = ()


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def make_overlay(base: bytes, patches: list[Patch]) -> dict:
    last_end=-1
    ordered=sorted(patches,key=lambda p:p.offset)
    for p in ordered:
        raw=bytes.fromhex(p.data_hex)
        if p.offset < 0 or p.offset+len(raw) > len(base): raise ValueError("patch out of range")
        if p.offset < last_end: raise ValueError("overlapping patches")
        if p.level not in {"PROVEN","CORROBORATED","INFERRED"}: raise ValueError("overlay patch needs explicit reconstruction level")
        last_end=p.offset+len(raw)
    body={"base_image_sha256":digest(base),"regions":[asdict(p) for p in ordered]}
    body["overlay_sha256"]=hashlib.sha256(json.dumps(body,sort_keys=True,separators=(",",":")).encode()).hexdigest()
    return body


def apply_overlay(base: bytes, overlay: dict) -> bytes:
    if digest(base) != overlay["base_image_sha256"]: raise ValueError("base image digest mismatch")
    out=bytearray(base)
    for p in overlay["regions"]:
        raw=bytes.fromhex(p["data_hex"]); off=int(p["offset"]); out[off:off+len(raw)]=raw
    return bytes(out)
