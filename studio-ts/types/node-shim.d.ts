declare module "node:test" { const test: (name:string, fn:()=>void|Promise<void>)=>void; export default test; }
declare module "node:assert/strict" { const assert: { equal(a:unknown,b:unknown):void; ok(v:unknown):void }; export default assert; }
