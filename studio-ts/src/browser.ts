import {recoveryBar,summarize} from "./visualize.js";
import type {Region} from "./model.js";
const sample:Region[]=[{offset:0,length:10,level:"PHYSICAL",confidence:1,entropy:0},{offset:10,length:4,level:"PROVEN",confidence:1,entropy:0},{offset:14,length:3,level:"INFERRED",confidence:.78,entropy:.42},{offset:17,length:2,level:"LOST",confidence:0,entropy:1}];
const el=document.querySelector("#app"); if(el) el.textContent=`${recoveryBar(sample)}\n${JSON.stringify(summarize(sample),null,2)}`;
