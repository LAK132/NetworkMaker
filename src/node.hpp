#include <stdlib.h>
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <cstring>
using std::to_string;
#include <map>
using std::map;
#include <thread>
using std::thread;
#include <iterator>
using std::distance;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::flush;

#include "json.hpp"
#include "property.hpp"

#ifndef NODE_H
#define NODE_H

class Node;
class Link;

class NodeTree 
{
public:
	uint64_t id;
	vector<Node*> node;
	vector<Link*> link;
	NodeTree(uint64_t ntid, JSON* json);
	void load(JSON* json);
	template<typename N, typename S>
	void init(JSON* json);
	void save(JSON* json, size_t pos);
};

class Socket;

class Node {
public:
	NodeTree* nodetree;
	uint64_t id;
	Property data;
	vector<Socket*> input;
	vector<Socket*> output;
	Node(NodeTree* nt, uint64_t nid, JSON* json = 0);
	~Node();
	Socket* addSocket(bool isIn, JSON* json = 0);
	inline Socket* addInput(JSON* json = 0);
	inline Socket* addOutput(JSON* json = 0);
	void load(JSON* json);
	template<typename N, typename S>
	void init(JSON* json);
	void save(JSON* json, size_t pos);
};

class Socket {
public:
	uint64_t id;
	Node* node;
	bool input;
	Link* link;
	bool linked = false;
	Property data;
	Socket(Node* n, uint64_t sid, bool isIn, JSON* json = 0);
	~Socket();
	void load(JSON* json);
	template<typename S>
	void init(JSON* json);
	void save(JSON* json, size_t pos);
};

class Link {
	public:
	Socket* to;
	Socket* from;
	Link(Socket* f, Socket* t);
	~Link();
	void save(JSON* json, size_t pos);
};

#include "node_temp.hpp"

#endif