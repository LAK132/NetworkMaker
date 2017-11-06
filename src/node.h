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

uint64_t strtoi64(const string str)
{
	uint64_t rtn = 0;
	for (size_t i = 0; str[i] != 0; i++)
	{
		rtn *= 10;
		rtn += str[i] - '0';
	}
	return rtn;
}

string i64tostr(uint64_t in)
{
	string rtn = "";
	do {
		rtn = (char)((in % 10) + '0') + rtn;
		in /= 10;
	} while (in != 0);

	return rtn;
}

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
		//cout << "count: " << data.size() << endl;
        return data.size();
    }
    static int callback(void* caller, int argc, char** argv, char** azColName)
    {
		SQL* c = (SQL*) caller;
        for (size_t i = 0; i < argc; i++)
        {
			if(c->data.count(azColName[i]) == 0) c->data[azColName[i]] = vector<string>(0);
			c->data[azColName[i]].push_back((argv[i] ? argv[i] : "0"));
			//cout << azColName[i] << ": " << argv[i] << endl;
        }
        return 0;
    }
};

class Node;
class Link;

class NodeTree 
{
public:
	uint64_t id;
	vector<Node*> node;
	vector<Link*> link;
	NodeTree(uint64_t ntid, SQL* sql)
	{
		id = ntid;
		if (sql != 0) load(sql);
	}
	void load(SQL* sql);
};

class Socket;

class Node {
public:
	NodeTree* nodetree;
	uint64_t id;
	vector<Socket*> input;
	vector<Socket*> output;
	Property data;
	Node(NodeTree* nt, uint64_t nid, SQL* sql = 0)
	{
		nodetree = nt;
		id = nid;
		if (sql != 0) load(sql);
	}
	~Node();
	Socket* addSocket(bool isIn, SQL* sql = 0);
	inline Socket* addInput(SQL* sql = 0) {return addSocket(true, sql);}
	inline Socket* addOutput(SQL* sql = 0) {return addSocket(false, sql);}
	void load(SQL* sql);
};

class Socket {
public:
	uint64_t id;
	Node* node;
	bool input;
	Link* link;
	Property data;
	Socket(Node* n, uint64_t sid, bool isIn, SQL* sql = 0)
	{
		node = n;
		id = sid;
		input = isIn;
		link = 0;
		if (sql != 0) load(sql);
	}
	~Socket();
	void load(SQL* sql)
	{
		/*string str = "select Link.FromNode, Link.FromSocket, Link.ToNode, Link.ToSocket from Link join Socket join Node on Socket.Node == Node.ID ";
		str += "where Node.Tree == '";
		str += to_string(node->nodetree->id);
		str += "' and Node.ID == '";
		str += to_string(node->id);
		str += "' and Socket.ID == '";
		str += to_string(id);
		str += "'";
		sql->exec(str);
		
		if (input)
		{
			uin64_t fnid = strtoi64(sql->data["FromNode"][0]);
			uin64_t fsid = strtoi64(sql->data["FromSocket"][0]);
			if (node->nodetree->node.size() > fnid && node->nodetree->node[fnid]->output.size() > fsid)
			{

			}
		}*/
	}
};

class Link {
	public:
	Socket* to;
	Socket* from;
	Link(Socket* f, Socket* t)
	{
		cout << "Add link " << f << " " << t << endl;
		to = t;
		t->link = this;
		from = f;
		f->link = this;
	}
	~Link(){}
};

Node::~Node()
{
	for (auto it = input.begin(); it != input.end(); it++) delete *it;
	for (auto it = output.begin(); it != output.end(); it++) delete *it;
}

Socket* Node::addSocket(bool isIn, SQL* sql)
{
	vector<Socket*>* sock = (isIn ? &input : &output);
	Socket* rtn = new Socket(this, sock->size(), isIn, sql);
	sock->push_back(rtn);
	return rtn;
}

Socket::~Socket()
{
	if (link != 0 && link->to == this) 
	{
		delete link;
	}
}

void Node::load(SQL* sql)
{
	string str = "select Input "\
		"from Socket "\
		"where Socket.Tree == ";
	str += i64tostr(nodetree->id);
	str += " and Socket.Node == ";
	str += i64tostr(id);
	sql->exec(str);
	
	uint64_t numSockets = sql->data["Input"].size();
	cout << numSockets << endl;
	uint64_t numInputs = 0;
	uint64_t numOutputs = 0;
	for (uint64_t i = 0; i < numSockets; i++)
	{
		if (sql->data["Input"][i][0] == '1') numInputs++;
		else numOutputs++;
	}
	input.resize(numInputs);
	output.resize(numOutputs);
	for (uint64_t i = 0; i < numInputs; i++)
	{
		cout << "Add input\n";
		input[i] = new Socket(this, i, sql);
	}
	for (uint64_t i = 0; i < numOutputs; i++)
	{
		cout << "Add output\n";
		output[i] = new Socket(this, i, sql);
	}
}

void NodeTree::load(SQL* sql)
{
	string str = "select count(ID) as numNodes "\
		"from Node "\
		"where Tree == ";
	str += i64tostr(id);
	sql->exec(str);

	cout << i64tostr(id) << endl << "2 1\n";
	
	uint64_t numNodes = strtoi64(sql->data["numNodes"][0]);
	node.resize(numNodes);
	for (uint64_t i = 0; i < numNodes; i++)
	{
		node[i] = new Node(this, i, sql);
	}
	
	//cout << "2 2\n";

	str = "select count(Link.FromNode) as numLinks "\
		"from Link join Node "\
		"on Link.Tree == Node.Tree ";
	str += "where Node.Tree == ";
	str += i64tostr(id);
	sql->exec(str);
	
	//cout << "2 3\n";

	uint64_t lsize = strtoi64(sql->data["numLinks"][0]);
	//cout << lsize << endl;
	link.resize(lsize);
	
	//cout << "2 4\n";

	uint64_t lid = 0;
	for (auto nit = node.begin(); nit != node.end(); nit++)
	{
		for (auto sit = (*nit)->input.begin(); sit != (*nit)->input.end(); sit++)
		{
			str = "select FromNode, FromSocket from Link "\
				"where Tree == ";
			str += i64tostr(id);
			str += " and ToNode == ";
			str += i64tostr((*nit)->id);
			str += " and ToSocket == ";
			str += i64tostr((*sit)->id);
			
			if(sql->exec(str))
			{
				uint64_t fnid = strtoi64(sql->data["FromNode"][0]);
				uint64_t fsid = strtoi64(sql->data["FromSocket"][0]);
				//cout << fnid << " " << node.size() << " " << fsid << " " << node[fnid]->output.size() << endl;
				if (node.size() > fnid && node[fnid]->output.size() > fsid)
				{
					link[lid++] = new Link(node[fnid]->output[fsid], (*sit));
				}
			}
		}
	}
}
