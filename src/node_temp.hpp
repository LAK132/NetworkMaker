template <typename T>
Node* NodeType(NodeTree* nt, uint64_t nid, JSON* json) {
	return new T(nt, nid, json); 
}

template <typename T>
Socket* SocketType(Node* n, uint64_t sid, bool isIn, JSON* json) { 
	return new T(n, sid, isIn, json); 
}