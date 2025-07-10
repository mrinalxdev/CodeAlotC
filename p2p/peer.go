package main

import (
	"bufio"
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"net"
	"os"
	"sync"
	// "time"
)

type Message struct {
	Type    string `json:"type"`
	Sender  string `json:"sender"`
	Content string `json:"content"`
	Hash    string `json:"hash"`
}

var peers = make(map[string]net.Conn)
var mu sync.Mutex
const peerFile = "peers.json"

func handleConnection(conn net.Conn) {
	defer conn.Close()
	addr := conn.RemoteAddr().String()
	mu.Lock()
	peers[addr] = conn
	savePeers()
	mu.Unlock()

	scanner := bufio.NewScanner(conn)
	for scanner.Scan() {
		msgData := scanner.Text()
		var msg Message
		if err := json.Unmarshal([]byte(msgData), &msg); err != nil {
			fmt.Println("Invalid message format:", err)
			continue
		}
		if !verifyMessage(msg) {
			fmt.Println("Invalid message signature from", addr)
			continue
		}
		handleMessage(msg, addr)
	}

	mu.Lock()
	delete(peers, addr)
	savePeers()
	mu.Unlock()
}

func broadcast(msg Message) {
	mu.Lock()
	defer mu.Unlock()
	msg.Hash = signMessage(msg)
	msgData, _ := json.Marshal(msg)
	for _, conn := range peers {
		fmt.Fprintln(conn, string(msgData))
	}
}

func signMessage(msg Message) string {
	h := sha256.New()
	h.Write([]byte(msg.Sender + msg.Content))
	return hex.EncodeToString(h.Sum(nil))
}

func verifyMessage(msg Message) bool {
	return signMessage(msg) == msg.Hash
}

func savePeers() {
	peerList := []string{}
	for addr := range peers {
		peerList = append(peerList, addr)
	}
	data, _ := json.Marshal(peerList)
	_ = os.WriteFile(peerFile, data, 0644)
}

func loadPeers() {
	data, err := os.ReadFile(peerFile)
	if err != nil {
		return
	}
	var peerList []string
	if err := json.Unmarshal(data, &peerList); err != nil {
		return
	}
	for _, addr := range peerList {
		go connectToPeer(addr)
	}
}

func startServer(port string) {
	ln, err := net.Listen("tcp", ":"+port)
	if err != nil {
		panic(err)
	}
	defer ln.Close()
	fmt.Println("Server started on port", port)
	for {
		conn, err := ln.Accept()
		if err == nil {
			go handleConnection(conn)
		}
	}
}

func connectToPeer(address string) {
	conn, err := net.Dial("tcp", address)
	if err != nil {
		fmt.Println("Failed to connect to peer:", err)
		return
	}
	mu.Lock()
	peers[address] = conn
	savePeers()
	mu.Unlock()
	go handleConnection(conn)
}

func handleMessage(msg Message, sender string) {
	switch msg.Type {
	case "text":
		fmt.Printf("%s: %s\n", sender, msg.Content)
	case "peer_update":
		go connectToPeer(msg.Content)
	}
}
