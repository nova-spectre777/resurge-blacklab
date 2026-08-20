package api
import("net/http/httptest";"strings";"testing")
func TestHealth(t *testing.T){ r:=httptest.NewRequest("GET","/health",nil); w:=httptest.NewRecorder(); New().Handler().ServeHTTP(w,r); if w.Code!=200||!strings.Contains(w.Body.String(),"MERA-OmegaX"){t.Fatalf("%d %s",w.Code,w.Body.String())} }
func TestRank(t *testing.T){ body:=`[{"offset":1,"length":4096,"information_gain":0.9,"dependency_impact":0.8,"evidence_uniqueness":0.9,"media_stress":0.2}]`; r:=httptest.NewRequest("POST","/v1/acquisition/rank",strings.NewReader(body)); w:=httptest.NewRecorder(); New().Handler().ServeHTTP(w,r); if w.Code!=200||!strings.Contains(w.Body.String(),"priority"){t.Fatalf("%d %s",w.Code,w.Body.String())} }
