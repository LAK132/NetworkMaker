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

Node::Node(NodeTree* nt, uint64_t nid, JSON* json)
{
    nodetree = nt;
    id = nid;
    if (json != 0) load(json);
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
	to = t;
	t->link = this;
	t->linked = true;
    from = f;
    f->link = this;
	f->linked = true;
}

Link::~Link()
{
	to->linked = false;
	from->linked = false;
}

Node::~Node()
{
	for (auto it = input.begin(); it != input.end(); it++) delete *it;
	for (auto it = output.begin(); it != output.end(); it++) delete *it;
}

Socket::~Socket()
{
	if (link != 0 && link->to == this) 
	{
		delete link;
	}
}

Socket* Node::addSocket(bool isIn, JSON* json)
{
	vector<Socket*>* sock = (isIn ? &input : &output);
	Socket* rtn = new Socket(this, sock->size(), isIn, json);
	sock->push_back(rtn);
	return rtn;
}

inline Socket* Node::addInput(JSON* json) 
{
    return addSocket(true, json);
}

inline Socket* Node::addOutput(JSON* json) 
{
    return addSocket(false, json);
}

void NodeTree::load(JSON* json)
{
	JSON& nodetree_j = (*json)("nodetree")[id];

	uint64_t numNodes = nodetree_j("node").arrSize();
	node.resize(numNodes);
	for (uint64_t i = 0; i < numNodes; i++)
	{
		node[i] = new Node(this, i, &nodetree_j);
	}

	JSON& lnkarr = nodetree_j("link");
	uint64_t lsize = lnkarr.arrSize();
	link.resize(lsize);

	uint64_t lid = 0;
	for (auto nit = node.begin(); nit != node.end(); nit++)
	{
		JSON& node_j = nodetree_j("node")[(*nit)->id];
		for (auto sit = (*nit)->input.begin(); sit != (*nit)->input.end(); sit++)
		{
			JSON& socket_j = node_j("input")[(*sit)->id];

			bool isLink = (*sit)->linked && lnkarr.size() > 0;
			uint64_t linkid = 0;

			while(isLink && !(lnkarr[linkid]("toNode").get<uint64_t>() == (*nit)->id &&
							lnkarr[linkid]("toSocket").get<uint64_t>() == (*sit)->id)) 
			{
				linkid++;
				if (linkid >= lnkarr.size()) isLink = false;
			}

			if(isLink)
			{
				JSON& link_j = nodetree_j("link")[linkid];
				uint64_t fnid = link_j("fromNode").get<uint64_t>();
				uint64_t fsid = link_j("fromSocket").get<uint64_t>();
				if (node.size() > fnid && node[fnid]->output.size() > fsid)
				{
					link[lid++] = new Link(node[fnid]->output[fsid], (*sit));
				}
			}
		}
	}
}

void Node::load(JSON* json)
{
	JSON& inputs_j = (*json)("node")[id]("input");
	uint64_t numInputs = inputs_j.arrSize();
	input.resize(numInputs);
	for (uint64_t i = 0; i < numInputs; i++)
	{
		input[i] = new Socket(this, i, true, &inputs_j);
	}

	JSON& outputs_j = (*json)("node")[id]("output");
	uint64_t numOutputs = outputs_j.arrSize();
	output.resize(numOutputs);
	for (uint64_t i = 0; i < numOutputs; i++)
	{
		output[i] = new Socket(this, i, false, &outputs_j);
	}
}

void Socket::load(JSON* json)
{
	JSON& socket_j = (*json)[id];
	input = socket_j("input").get<bool>();
	linked = socket_j("linked").get<bool>();
}

void NodeTree::save(JSON* json, size_t pos)
{
	id = pos;
	JSON& nodetree_l = (*json)("nodetree");
	if(nodetree_l.arrSize() <= id)
	{
		for(size_t i = nodetree_l.arrSize(); i <= id; i++)
		{
			nodetree_l.push_back(JSON());
		}
	}
	JSON& nodetree_j = nodetree_l[id];
	for(auto it = node.begin(); it != node.end(); it++)
	{
		(*it)->save(&nodetree_j, distance(node.begin(), it));
	}
	for(auto it = link.begin(); it != link.end(); it++)
	{
		(*it)->save(&nodetree_j, distance(link.begin(), it));
	}
}

void Node::save(JSON* json, size_t pos)
{
	id = pos;
	JSON& node_l = (*json)("node");
	if(node_l.arrSize() <= id)
	{
		for(size_t i = node_l.arrSize(); i <= id; i++)
		{
			node_l.push_back(JSON());
		}
	}
	JSON& node_j = node_l[id];
	node_j("data").set(data.getv());

	for(auto it = input.begin(); it != input.end(); it++)
	{
		(*it)->save(&node_j("input"), distance(input.begin(), it));
	}
	for(auto it = output.begin(); it != output.end(); it++)
	{
		(*it)->save(&node_j("output"), distance(output.begin(), it));
	}
}

void Socket::save(JSON* json, size_t pos)
{
	id = pos;
	JSON& socket_l = (*json);
	if(socket_l.arrSize() <= id)
	{
		for(size_t i = socket_l.arrSize(); i <= id; i++)
		{
			socket_l.push_back(JSON());
		}
	}
	JSON& socket_j = (*json)[id];
	socket_j("input").set(input);
	socket_j("linked").set(linked);
	socket_j("data").set(data.getv());
}

void Link::save(JSON* json, size_t pos)
{
	JSON& link_l = (*json)("link");
	if(link_l.arrSize() <= pos)
	{
		for(size_t i = link_l.arrSize(); i <= pos; i++)
		{
			link_l.push_back(JSON());
		}
	}
	JSON& link_j = link_l[pos];
	link_j("toNode").set(to->node->id);
	link_j("toSocket").set(to->id);
	link_j("fromNode").set(from->node->id);
	link_j("fromSocket").set(from->id);
}