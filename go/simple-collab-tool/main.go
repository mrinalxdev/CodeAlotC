package main

import (
	"embed"
	"encoding/json"
	"html/template"
	"log"
	"net/http"
)

var templateFS embed.FS

var staticFS embed.FS

type Block struct {
	ID    string `json:"id"`
	Type  string `json:"type"`
	Label string `json:"label"`
}

var availableBlocks = []Block{
	{ID: "get", Type: "endpoint", Label: "GET"},
	{ID: "post", Type: "endpoint", Label: "POST"},
	{ID: "put", Type: "endpoint", Label: "PUT"},
	{ID: "delete", Type: "endpoint", Label: "DELETE"},
	{ID: "param", Type: "parameter", Label: "Parameter"},
	{ID: "response", Type: "response", Label: "Response"},
}

func main() {
	http.HandleFunc("/", handleIndex)
	http.HandleFunc("/api/blocks", handleBlocks)
	http.Handle("/static/", http.FileServer(http.FS(staticFS)))

	log.Println("Server starting on :8080")
	log.Fatal(http.ListenAndServe(":8080", nil))
}

func handleIndex(w http.ResponseWriter, r *http.Request) {
	tmpl, err := template.ParseFS(templateFS, "index.html")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	err = tmpl.Execute(w, nil)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

func handleBlocks(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(availableBlocks)
}