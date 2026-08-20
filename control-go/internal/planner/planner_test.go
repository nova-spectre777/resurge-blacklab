package planner
import (
 "testing"
 "github.com/nova-spectre777/resurge-blacklab/control-go/internal/model"
)
func TestRank(t *testing.T){ r:=Rank([]model.ReadTarget{{Offset:1,InfoGain:.9,Impact:.9,Uniqueness:.9,MediaStress:.2},{Offset:2,InfoGain:.1,Impact:.1,Uniqueness:.1,MediaStress:.1}}); if r[0].Offset!=1{t.Fatalf("unexpected rank: %+v",r)} }
func TestCertificateStableShape(t *testing.T){ c,err:=Certificate("image","overlay",[]model.Region{{Offset:1,Length:2,Level:model.Proven,Confidence:1}}); if err!=nil||len(c.Digest)!=64||c.Algorithm==""{t.Fatalf("bad certificate: %+v %v",c,err)} }
