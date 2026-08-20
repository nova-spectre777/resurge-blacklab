package model

import "time"

type TruthLevel string

const (
	Physical     TruthLevel = "PHYSICAL"
	Proven       TruthLevel = "PROVEN"
	Corroborated TruthLevel = "CORROBORATED"
	Inferred     TruthLevel = "INFERRED"
	Ambiguous    TruthLevel = "AMBIGUOUS"
	Lost         TruthLevel = "LOST"
)

type Region struct {
	Offset     uint64     `json:"offset"`
	Length     uint64     `json:"length"`
	Level      TruthLevel `json:"level"`
	Confidence float64    `json:"confidence"`
	Entropy    float64    `json:"entropy"`
	Candidate  string     `json:"candidate,omitempty"`
	Evidence   []string   `json:"evidence,omitempty"`
}

type Certificate struct {
	Version       string    `json:"version"`
	ImageDigest   string    `json:"image_digest"`
	OverlayDigest string    `json:"overlay_digest"`
	Algorithm     string    `json:"algorithm"`
	Regions       []Region  `json:"regions"`
	CreatedAt     time.Time `json:"created_at"`
	Digest        string    `json:"digest"`
}

type ReadTarget struct {
	Offset       uint64  `json:"offset"`
	Length       uint64  `json:"length"`
	InfoGain     float64 `json:"information_gain"`
	Impact       float64 `json:"dependency_impact"`
	Uniqueness   float64 `json:"evidence_uniqueness"`
	MediaStress  float64 `json:"media_stress"`
	Priority     float64 `json:"priority"`
}
