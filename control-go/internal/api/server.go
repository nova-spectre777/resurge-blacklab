package api

import (
	"encoding/json"
	"net/http"

	"github.com/nova-spectre777/resurge-blacklab/control-go/internal/model"
	"github.com/nova-spectre777/resurge-blacklab/control-go/internal/planner"
)

type Server struct{ mux *http.ServeMux }

func New() *Server {
	s := &Server{mux: http.NewServeMux()}
	s.mux.HandleFunc("GET /health", func(w http.ResponseWriter, _ *http.Request) {
		_ = json.NewEncoder(w).Encode(map[string]any{"ok":true,"algorithm":"MERA-OmegaX-Prime/0.1","mode":"read-only-foundation"})
	})
	s.mux.HandleFunc("POST /v1/acquisition/rank", s.rank)
	s.mux.HandleFunc("POST /v1/certificates", s.certificate)
	return s
}
func (s *Server) Handler() http.Handler { return s.mux }

func (s *Server) rank(w http.ResponseWriter, r *http.Request) {
	var targets []model.ReadTarget
	if err := json.NewDecoder(http.MaxBytesReader(w,r.Body,1<<20)).Decode(&targets); err != nil { http.Error(w,"invalid json",400); return }
	_ = json.NewEncoder(w).Encode(planner.Rank(targets))
}

func (s *Server) certificate(w http.ResponseWriter, r *http.Request) {
	var in struct{ ImageDigest string `json:"image_digest"`; OverlayDigest string `json:"overlay_digest"`; Regions []model.Region `json:"regions"` }
	if err := json.NewDecoder(http.MaxBytesReader(w,r.Body,4<<20)).Decode(&in); err != nil { http.Error(w,"invalid json",400); return }
	if in.ImageDigest=="" || in.OverlayDigest=="" { http.Error(w,"digests required",400); return }
	c, err := planner.Certificate(in.ImageDigest,in.OverlayDigest,in.Regions); if err!=nil {http.Error(w,"certificate error",500);return}
	_ = json.NewEncoder(w).Encode(c)
}
