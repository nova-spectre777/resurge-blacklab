export type TruthLevel = "PHYSICAL"|"PROVEN"|"CORROBORATED"|"INFERRED"|"AMBIGUOUS"|"LOST";
export interface Region { offset:number; length:number; level:TruthLevel; confidence:number; entropy:number; candidate?:string; evidence?:string[] }
export interface Certificate { version:string; image_digest:string; overlay_digest:string; algorithm:string; regions:Region[]; created_at:string; digest:string }
export const glyph: Record<TruthLevel,string> = {PHYSICAL:"█",PROVEN:"▓",CORROBORATED:"▒",INFERRED:"░",AMBIGUOUS:"?",LOST:"·"};
