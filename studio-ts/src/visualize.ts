import {glyph, type Region, type TruthLevel} from "./model.js";
export function recoveryBar(regions: Region[], width=48): string {
  if(regions.length===0) return "";
  const total=regions.reduce((n,r)=>n+r.length,0); let out="";
  for(const r of regions){ const n=Math.max(1,Math.round((r.length/total)*width)); out+=glyph[r.level].repeat(n); }
  return out.slice(0,width).padEnd(width,"·");
}
export function summarize(regions: Region[]): Record<TruthLevel,number>{
 const result={PHYSICAL:0,PROVEN:0,CORROBORATED:0,INFERRED:0,AMBIGUOUS:0,LOST:0} satisfies Record<TruthLevel,number>;
 for(const r of regions) result[r.level]+=r.length; return result;
}
export function entropyHotspots(regions: Region[]): Region[]{ return [...regions].sort((a,b)=>b.entropy-a.entropy || a.offset-b.offset); }
