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

class NodeGraph : public Prop {
public:
	//ImVec2 scrolling = ImVec2(0.0f, 0.0f);
	//bool showGrid = true;
	NodeTree* nodeTree;

	NodeGraph(NodeTree* nt);

	//bool render(ImDrawList* drawList, ImVec2 offset = ImVec2(0.0f, 0.0f));
};

//A NodeMaker instance is a NodeType<T> constructor saved with type information
//i.e. NodeMaker nm = NodeType<DefaultNode>;
//Used for calling a constructor without specifying the template type
typedef Node*(*NodeMaker)(NodeTree*, uint64_t, JSON*);
template <typename T> Node* NodeType(NodeTree* nt, uint64_t nid, JSON* json);
//Global list of all Node constructors
extern map<string, NodeMaker> MakeNode;

class NodeTree : public Data, public Prop {
protected:
    void _removeLink(Link* lnk);
    friend class Link;
public:
	bool draggingLink = false;
	Socket* dragFromPtr = 0;
	Socket* dragToPtr = 0;

	uint64_t id;			//Unique ID of this NodeTree in its JSON stream
	vector<Node*> node;		//Pointers to all Nodes in this NodeTree
	vector<Link*> link;		//Pointers to all Socket Links in this NodeTree
	string name;			//Non-unique name

	NodeTree(uint64_t ntid, JSON* json);
	~NodeTree();

	Node* addNode(const string& nodeType, JSON* json = 0);	//Add Node with nodeType
	Node* addNode(JSON* json);								//Add Node with JSON("type")
	Link* addLink(Socket* from, Socket* to);
	void removeLink(Link* lnk);

	void load(JSON& nodetree_j);	//Load NodeTree data from JSON stream
	void save(JSON& nodetree_j);	//Save NodeTree data to JSON stream
	//bool render(ImDrawList* drawList, ImVec2 offset = ImVec2(0.0f, 0.0f));
	void poll();
};

//A SocketMaker instance is a SocketType<T> constructor saved with type information
//i.e. SocketMaker sm = SocketType<DefaultSocket>;
//Used for calling a constructor without specifying the template type
typedef Socket*(*SocketMaker)(Node*, uint64_t, bool, JSON*);
template <typename T> Socket* SocketType(Node* n, uint64_t sid, bool isIn, JSON* json);
//Global list of all Socket constructors
extern map<string, SocketMaker> MakeSocket;

class Node : public Data, public Prop {
public:
	string type;			//The type name of this Node instance
	NodeTree* nodetree;		//Pointer to the parent NodeTree
	uint64_t id;			//Unique ID of this Node in its NodeTree
	vector<Socket*> input;	//Pointers to all Input Sockets for this Node
	vector<Socket*> output;	//Pointers to all Output Sockets for this Node
	string name;			//Non-unique name
    bool polled = false;

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
	//bool render(ImDrawList* drawList, ImVec2 offset = ImVec2(0.0f, 0.0f));

	virtual void loadData(JSON& data_j);	//Load Derived Node class data from JSON stream
	virtual void saveData(JSON& data_j);	//Save Derived Node class data tp JSON stream
	virtual Node* poll();					//Poll the Node to recalculate its outputs
	//virtual void draw(bool& updt);
};

class Socket : public Data, public Prop {
public:
    bool isHovered = false;

	string type;			//The type name of this Socket instance
	Node* node;				//Pointer to the parent Node
	uint64_t id;			//Unique ID of this Socket in its Node
	bool input;				//If this Socket is an input in the parent Node
	Link* link;				//Pointers to Link for this Socket
	bool linked = false;	//If this Socket is Linked
	string name;			//Non-unique name

	Socket(Node* n, uint64_t sid, bool isIn, JSON* json = 0);
	~Socket();

	Link* addLink(Socket* other);

	void load(JSON& socket_j);	//Load Socket data from JSON stream
	void save(JSON& socket_j);	//Save Socket data to JSON stream
	//bool render(ImDrawList* drawList, ImVec2 offset = ImVec2(0.0f, 0.0f));

	virtual void loadData(JSON& data_j);	//Load Derived Socket class data from JSON stream
	virtual void saveData(JSON& data_j);	//Save Derived Socket class data to JSON stream
	//virtual void draw(bool& updt);
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
	//bool render(ImDrawList* drawList, ImVec2 offset = ImVec2(0.0f, 0.0f));
};

#include "node_temp.hpp"

#endif
