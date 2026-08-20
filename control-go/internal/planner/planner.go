package planner

import (
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"math"
	"sort"
	"time"

	"github.com/nova-spectre777/resurge-blacklab/control-go/internal/model"
)

func Rank(targets []model.ReadTarget) []model.ReadTarget {
	out := append([]model.ReadTarget(nil), targets...)
	for i := range out {
		stress := math.Max(0.02, out[i].MediaStress)
		out[i].Priority = (math.Max(0, out[i].InfoGain) * (0.5 + math.Max(0, out[i].Impact)) * (0.5 + math.Max(0, out[i].Uniqueness))) / stress
	}
	sort.SliceStable(out, func(i, j int) bool { return out[i].Priority > out[j].Priority })
	return out
}

func Certificate(imageDigest, overlayDigest string, regions []model.Region) (model.Certificate, error) {
	c := model.Certificate{
		Version: "resurge-certificate/v1", ImageDigest: imageDigest, OverlayDigest: overlayDigest,
		Algorithm: "MERA-OmegaX-Prime/0.1", Regions: append([]model.Region(nil), regions...), CreatedAt: time.Now().UTC(),
	}
	canonical := struct {
		Version, ImageDigest, OverlayDigest, Algorithm string
		Regions []model.Region
	}{c.Version, c.ImageDigest, c.OverlayDigest, c.Algorithm, c.Regions}
	b, err := json.Marshal(canonical)
	if err != nil { return c, err }
	h := sha256.Sum256(b)
	c.Digest = hex.EncodeToString(h[:])
	return c, nil
}
