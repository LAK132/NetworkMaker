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
#include <memory>
using std::unique_ptr;
//#include <SFML/Graphics.hpp>
//using sf::Vector2f;

#include "json.hpp"
#include "property.hpp"
#include "prop.hpp"

#ifndef NODE_H
#define NODE_H

class NodeTree;
class Node;
class Socket;
class Link;

class Data {
public:
	virtual void load(JSON& json) =0;
	virtual void save(JSON& json) =0;
};

typedef Node*(*NodeMaker)(NodeTree*, uint64_t, JSON*);
template <typename T>
Node* NodeType(NodeTree* nt, uint64_t nid, JSON* json);
extern map<string, NodeMaker> MakeNode;

class NodeTree : public Data, public Prop {
public:
	uint64_t id;			//Unique ID of this NodeTree in its JSON stream
	vector<Node*> node;		//Pointers to all Nodes in this NodeTree
	vector<Link*> link;		//Pointers to all Socket Links in this NodeTree

	NodeTree(uint64_t ntid, JSON* json);
	~NodeTree();

	Node* addNode(const string& nodeType, JSON* json = 0);	//Add Node with nodeType
	Node* addNode(JSON* json);								//Add Node with JSON("type")
	Link* addLink(Socket* from, Socket* to);

	void load(JSON& nodetree_j);	//Load NodeTree data from JSON stream
	void save(JSON& nodetree_j);	//Save NodeTree data to JSON stream
	bool render();
};

typedef Socket*(*SocketMaker)(Node*, uint64_t, bool, JSON*);
template <typename T>
Socket* SocketType(Node* n, uint64_t sid, bool isIn, JSON* json);
extern map<string, SocketMaker> MakeSocket;

class Node : public Data, public Prop {
public:
	string type;			//The type name of this Node instance
	NodeTree* nodetree;		//Pointer to the parent NodeTree
	uint64_t id;			//Unique ID of this Node in its NodeTree
	vector<Socket*> input;	//Pointers to all Input Sockets for this Node
	vector<Socket*> output;	//Pointers to all Output Sockets for this Node

	Node(NodeTree* nt, uint64_t nid, JSON* json = 0);
	~Node();

	//Add a new Socket to this Node
	Socket* addSocket(bool isIn, const string& sockType, JSON* json = 0);
	Socket* addSocket(bool isIn, JSON* json); 	//where type name is JSON("type")
	//Add a new Input Socket to this Node
	Socket* addInput(const string& sockType, JSON* json = 0);
	Socket* addInput(JSON* json);				//where type name is JSON("type")
	//Add a new Output Socket to this Node
	Socket* addOutput(const string& sockType, JSON* json = 0);
	Socket* addOutput(JSON* json);				//where type name is JSON("type")

	void load(JSON& node_j);	//Load Node data from JSON stream
	void save(JSON& node_j);	//Save Node data to JSON stream
	bool render();

	virtual void loadData(JSON& data_j) =0;	//Load Derived Node class data from JSON stream
	virtual void saveData(JSON& data_j) =0;	//Save Derived Node class data tp JSON stream
	virtual void poll() =0;					//Poll the Node to recalculate its outputs
	virtual void draw(bool& updt) =0;
};

class Socket : public Data, public Prop {
public:
	string type;
	uint64_t id;
	Node* node;
	bool input;
	Link* link;
	bool linked = false;

	Socket(Node* n, uint64_t sid, bool isIn, JSON* json = 0);
	~Socket();

	Link* addLink(Socket* other);

	void load(JSON& socket_j);	//Load Socket data from JSON stream
	void save(JSON& socket_j);	//Save Socket data to JSON stream
	bool render();

	virtual void loadData(JSON& data_j) =0;	//Load Derived Socket class data from JSON stream
	virtual void saveData(JSON& data_j) =0;	//Save Derived Socket class data to JSON stream
	virtual void draw(bool& updt) =0;
};

class Link : public Data, public Prop {
public:
	uint64_t id;
	Socket* to;
	Socket* from;
	Link(Socket* f, Socket* t);
	~Link();
	void load(JSON& link_j);	//Load Link data from JSON stream
	void save(JSON& link_j);	//Save Link data to JSON stream
	bool render();
};

#include "node_temp.hpp"

#endif
