package main

import (
	"log"
	"net/http"
	"os"
	"github.com/nova-spectre777/resurge-blacklab/control-go/internal/api"
)
func main(){ addr:=os.Getenv("RESURGE_ADDR"); if addr==""{addr=":8787"}; log.Printf("RESURGE control listening on %s",addr); log.Fatal(http.ListenAndServe(addr,api.New().Handler())) }
