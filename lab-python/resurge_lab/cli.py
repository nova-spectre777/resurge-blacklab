from __future__ import annotations
import argparse
from .arena import benchmark, report

def main():
    p=argparse.ArgumentParser(prog="resurge-lab")
    p.add_argument("benchmark",nargs="?")
    p.add_argument("--rate",type=float,default=.05)
    p.add_argument("--seed",type=int,default=11)
    p.add_argument("--cluster",type=int,default=16)
    a=p.parse_args(); print(report(benchmark(a.rate,a.seed,a.cluster)))
if __name__=="__main__": main()
