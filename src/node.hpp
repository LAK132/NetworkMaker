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
class Socket;
class Link;

class Data {
public:
	virtual void load(JSON& json) = 0;
	virtual void save(JSON& json) = 0;
};

class NodeTree : public Data {
public:
	uint64_t id;
	vector<Node*> node;
	vector<Link*> link;

	NodeTree(uint64_t ntid, JSON* json);
	~NodeTree();

	Node* addNode(JSON* json = 0);
	Link* addLink(Socket* from, Socket* to);

	template<typename N, typename S>
	void init(JSON& nodetree_j);
	void load(JSON& nodetree_j);
	void save(JSON& nodetree_j);
};

class Node : public Data {
public:
	NodeTree* nodetree;
	uint64_t id;
	Data* data;
	vector<Socket*> input;
	vector<Socket*> output;

	Node(NodeTree* nt, uint64_t nid, JSON* json = 0);
	~Node();

	Socket* addSocket(bool isIn, JSON* json = 0);
	Socket* addInput(JSON* json = 0);
	Socket* addOutput(JSON* json = 0);

	template<typename N, typename S>
	void init(JSON& node_j);
	void load(JSON& node_j);
	void save(JSON& node_j);
};

class Socket : public Data {
public:
	uint64_t id;
	Node* node;
	bool input;
	Link* link;
	bool linked = false;
	Data* data;

	Socket(Node* n, uint64_t sid, bool isIn, JSON* json = 0);
	~Socket();

	Link* addLink(Socket* other);

	template<typename S>
	void init(JSON& socket_j);
	void load(JSON& socket_j);
	void save(JSON& socket_j);
};

class Link : public Data {
public:
	uint64_t id;
	Socket* to;
	Socket* from;
	Link(Socket* f, Socket* t);
	~Link();
	void load(JSON& link_j);
	void save(JSON& link_j);
};

#include "node_temp.hpp"

#endif