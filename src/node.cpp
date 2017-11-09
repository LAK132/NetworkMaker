#include "node.hpp"

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

NodeTree::NodeTree(uint64_t ntid, JSON* json)
{
    id = ntid;
    if (json != 0) load(json);
}

Node::~Node()
{
	for (auto it = input.begin(); it != input.end(); it++) delete *it;
	for (auto it = output.begin(); it != output.end(); it++) delete *it;
}

Socket* Node::addSocket(bool isIn, JSON* json)
{
	vector<Socket*>* sock = (isIn ? &input : &output);
	Socket* rtn = new Socket(this, sock->size(), isIn, json);
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

void Node::load(JSON* json)
{
	JSON& inputs_j = (*json)("nodetree")[nodetree->id]("node")[id]("input");
	uint64_t numSockets = inputs_j.arrSize();
	//cout << numSockets << endl;
	uint64_t numInputs = 0;
	uint64_t numOutputs = 0;
	for (uint64_t i = 0; i < numSockets; i++)
	{
		if (inputs_j[i]("input")->get<bool>()) numInputs++;
		else numOutputs++;
	}
	input.resize(numInputs);
	output.resize(numOutputs);
	for (uint64_t i = 0; i < numInputs; i++)
	{
		cout << "Add input\n";
		input[i] = new Socket(this, i, json);
	}
	for (uint64_t i = 0; i < numOutputs; i++)
	{
		cout << "Add output\n";
		output[i] = new Socket(this, i, json);
	}
}

void NodeTree::load(JSON* json)
{
	JSON& nodetree_j = (*json)("nodetree")[id];
	cout << i64tostr(id) << endl << "2 1\n";
	
	uint64_t numNodes = nodetree_j("node").arrSize();
	cout << i64tostr(numNodes) << endl;
	node.resize(numNodes);
	for (uint64_t i = 0; i < numNodes; i++)
	{
		node[i] = new Node(this, i, json);
	}
	
	cout << "2 2\n";
	
	cout << "2 3\n";

	uint64_t lsize = nodetree_j("link").arrSize();
	cout << lsize << endl;
	link.resize(lsize);
	
	cout << "2 4\n";

	uint64_t lid = 0;
	for (auto nit = node.begin(); nit != node.end(); nit++)
	{
		for (auto sit = (*nit)->input.begin(); sit != (*nit)->input.end(); sit++)
		{			
			JSON& socket_j = nodetree_j("node")[(*nit)->id]("socket")[(*sit)->id];
			uint64_t fnid = socket_j("fromNode")->get<uint64_t>();
			uint64_t fsid = socket_j("fromSocket")->get<uint64_t>();
			cout << fnid << " " << node.size() << " " << fsid << " " << node[fnid]->output.size() << endl;
			if (node.size() > fnid && node[fnid]->output.size() > fsid)
			{
				link[lid++] = new Link(node[fnid]->output[fsid], (*sit));
			}
		}
	}
}

void Socket::load(JSON* json)
{

}

Node::Node(NodeTree* nt, uint64_t nid, JSON* json)
{
    nodetree = nt;
    id = nid;
    if (json != 0) load(json);
}

inline Socket* Node::addInput(JSON* json) {
    return addSocket(true, json);
}

inline Socket* Node::addOutput(JSON* json) {
    return addSocket(false, json);
}

Socket::Socket(Node* n, uint64_t sid, bool isIn, JSON* json)
{
    node = n;
    id = sid;
    input = isIn;
    link = 0;
    if (json != 0) load(json);
}

Link::Link(Socket* f, Socket* t)
{
    cout << "Add link " << f << " " << t << endl;
    to = t;
    t->link = this;
    from = f;
    f->link = this;
}