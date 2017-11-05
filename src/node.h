#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include <thread>

#include <iostream>
using std::cout;
using std::endl;

using std::vector;
using std::string;
using std::to_string;
using std::map;
using std::thread;

#include "property.h"

class SQL
{
public:
    sqlite3* db;
    char* zErrMsg;
    int rc;
    map<string, vector<string>> data;
    SQL(const char* dbloc)
    {
        rc = sqlite3_open(dbloc, &db);
    }
    size_t exec(string str)
    {
		data.clear();
		rc = sqlite3_exec(db, str.c_str(), callback, this, &(zErrMsg));
		cout << "count: " << data.size() << endl;
        return data.size();
    }
    static int callback(void* caller, int argc, char** argv, char** azColName)
    {
        SQL* c = (SQL*) caller;
        for (size_t i = 0; i < argc; i++)
        {
			if(c->data.count(azColName[i]) == 0) c->data[azColName[i]] = vector<string>(1);
            c->data[azColName[i]].push_back((argv[i] ? argv[i] : "0"));
        }
        return 0;
    }
};

class Node;

class NodeTree 
{
public:
	string name;
	vector<Node*> node;
	NodeTree(string ntName, SQL* sql)
	{
		name = ntName;
		if (sql != 0) load(sql);
	}
	void load(SQL* sql)
	{
		string str = "select count(Node.ID) as numNodes from NodeTree join Node on Node.Tree == NodeTree.ID where NodeTree.Name == '";
		str += name;
		str += "'";
		sql->exec(str);
		
		char* numNodesStr = sql->data["numNodes"][0].c_str();
		uint64_t numNodes = 0;
		for (size_t i = 0; numNodesStr[i] != 0; i++)
		{
			numNodes *= 10;
			numNodes += numNodesStr[i] - '0';
		}
		node.resize(numNodes);
		for (uint64_t i = 0; i < numNodes; i++)
		{
			node[i] = new Node(this, i, sql);
		}
	}
};

class Socket;
class Link;

class Node {
public:
	NodeTree* nodetree
	uint64_t id;
	vector<Socket*> input;
	Socket* output;
	Property data;
	Node(NodeTree* nt, uint64_t _id, SQL* sql)
	{
		nodetree = nt;
		id = _id;
		if (sql != 0) load(sql);
	}
	void load(SQL* sql)
	{
		string str = "select * from Node where NodeTree == '";
		str += nodetree->name;
		str += "' and ID == '";
		str += id;
		str += "'";
		sql->exec(str);
		
		char* numNodesStr = sql->data["numNodes"][0].c_str();
		uint64_t numNodes = 0;
		for (size_t i = 0; numNodesStr[i] != 0; i++)
		{
			numNodes *= 10;
			numNodes += numNodesStr[i] - '0';
		}
		node.resize(numNodes);
		for (uint64_t i = 0; i < numNodes; i++)
		{
			node[i] = new Node(this, i, sql);
		}
	}
	Node(size_t numInputs = 0, uint64_t iID = 0, uint64_t iTreeID = 0);
	~Node();
	template<typename N, typename S>
	void init(SQL* sql);
};

class Socket {
public:
	uint64_t ID;
	Node* parent;
	Link* link;
	Property data;
	Socket(Node* n, uint64_t iID = 0);
	~Socket();
	template<typename T>
	void init(SQL* sql);
};

class Link {
	public:
	Socket* to;
	Socket* from;
	Link(Socket* f, Socket* t);
	~Link();
};

Socket::Socket(Node* n, uint64_t iID) {
	ID = iID;
	parent = n;
	link = 0;
}

Socket::~Socket() {
	if (link != 0 && link->to == this) {
		//printf("delete link\n");
		delete link;
	}
}

template<typename T>
void Socket::init(SQL* sql) {
	string str = "select Socket.Data from Socket join Node on Socket.Node == Node.ID ";
	str += "where Node.ID == '";
	str += to_string((uint64_t)(parent->ID));
	str += "' and Node.Tree == '";
	str += to_string((uint64_t)(parent->TreeID));
	str += "' and Socket.ID == '";
	str += to_string((uint64_t)(ID));
	str += "'";
	cout << str << endl;

	sql->exec(str);
	
	data.init<T>(this, &(sql->data));
}

Link::Link(Socket* f, Socket* t) {
	to = t;
	t->link = this;
	from = f;
	f->link = this;
}

Link::~Link() {
}

Node::Node(size_t numInputs, uint64_t iID, uint64_t iTreeID) {
	ID = iID;
	TreeID = iTreeID;
	input.resize(numInputs);
	size_t i = 1;
	for (auto it = input.begin(); it != input.end(); it++) *it = new Socket(this, i++);
	output = new Socket(this, 0);
}

Node::~Node() {
	//printf("delete input\n");
	for (auto it = input.begin(); it != input.end(); it++) delete *it;
	//printf("delete output\n");
	delete output;
}

template<typename N, typename S>
void Node::init(SQL* sql) {
	string str = "select Node.Data from Node ";
	str += "where ID == '";
	str += to_string((uint64_t)(ID));
	str += "' and Tree == '";
	str += to_string((uint64_t)(TreeID));
	str += "'";
	cout << str << endl;

	sql->exec(str);
	
	data.init<N>(this, &(sql->data));
	for (auto it = input.begin(); it != input.end(); it++) (*it)->init<S>(sql);
	output->init<S>(sql);
	/*(*it)->data.init<S>((*it));
	output->data.init<S>(output);*/
}
/*
int main()
{
	Property p;
	p.set(1);
	p.getr<int>();
    return 0;
}
*/