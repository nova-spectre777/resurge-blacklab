import test from "node:test"; import assert from "node:assert/strict"; import {recoveryBar,summarize,entropyHotspots} from "../src/visualize.js";
const r=[{offset:0,length:8,level:"PHYSICAL" as const,confidence:1,entropy:0},{offset:8,length:2,level:"LOST" as const,confidence:0,entropy:1}];
test("bar has requested width",()=>assert.equal(recoveryBar(r,10).length,10));
test("summary counts bytes",()=>assert.equal(summarize(r).PHYSICAL,8));
test("hotspots sort high entropy first",()=>assert.equal(entropyHotspots(r)[0].level,"LOST"));
