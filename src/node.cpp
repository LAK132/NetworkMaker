#include "node.hpp"

uint64_t strtoi64(const string str) {
	uint64_t rtn = 0;
	for (size_t i = 0; str[i] != 0; i++)
	{
		rtn *= 10;
		rtn += str[i] - '0';
	}
	return rtn;
}

string i64tostr(uint64_t in) {
	string rtn = "";
	do {
		rtn = (char)((in % 10) + '0') + rtn;
		in /= 10;
	} while (in != 0);

	return rtn;
}

void Data::load(JSON& json){}

void Data::save(JSON& json){}

NodeTree::NodeTree(uint64_t ntid, JSON* json) {
    id = ntid;
    if (json != 0) load(*json);
}

Node::Node(NodeTree* nt, uint64_t nid, JSON* json) {
    nodetree = nt;
    id = nid;
    if (json != 0) load(*json);
}

Socket::Socket(Node* n, uint64_t sid, bool isIn, JSON* json) {
    node = n;
    id = sid;
    input = isIn;
    link = 0;
    if (json != 0) load(*json);
}

Link::Link(Socket* f, Socket* t) {
	to = t;
	t->link = this;
	t->linked = true;
    from = f;
    f->link = this;
	f->linked = true;
}

Link::~Link() {
	to->linked = false;
	from->linked = false;
}

Node::~Node() {
	for (auto it = input.begin(); it != input.end(); it++) delete *it;
	for (auto it = output.begin(); it != output.end(); it++) delete *it;
}

Socket::~Socket() {
	if (link != 0 && link->to == this) 
	{
		delete link;
	}
}

Socket* Node::addSocket(bool isIn, JSON* json) {
	vector<Socket*>* sock = (isIn ? &input : &output);
	Socket* rtn = new Socket(this, sock->size(), isIn, json);
	sock->push_back(rtn);
	return rtn;
}

inline Socket* Node::addInput(JSON* json) {
    return addSocket(true, json);
}

inline Socket* Node::addOutput(JSON* json) {
    return addSocket(false, json);
}

void NodeTree::load(JSON& nodetree_j) {
	JSON& node_l = nodetree_j("node");

	uint64_t numNodes = node_l.arrSize();
	node.resize(numNodes);
	for (uint64_t i = 0; i < numNodes; i++)
	{
		node[i] = new Node(this, i, &node_l[i]);
	}

	JSON& link_l = nodetree_j("link");

	uint64_t numLinks = link_l.arrSize();
	link.resize(numLinks);
	for (uint64_t i = 0; i < numLinks; i++)
	{
		JSON& link_j = link_l[i];
		Socket* from = node[link_j("fromNode").get<uint64_t>()]->output[link_j("fromSocket").get<uint64_t>()];
		Socket* to = node[link_j("toNode").get<uint64_t>()]->input[link_j("toSocket").get<uint64_t>()];
		link[i] = new Link(from, to);
	}
}

void Node::load(JSON& node_j) {
	JSON& inputs_l = node_j("input");
	uint64_t numInputs = inputs_l.arrSize();
	input.resize(numInputs);
	for (uint64_t i = 0; i < numInputs; i++)
	{
		input[i] = new Socket(this, i, true, &inputs_l[i]);
	}

	JSON& outputs_l = node_j("output");
	uint64_t numOutputs = outputs_l.arrSize();
	output.resize(numOutputs);
	for (uint64_t i = 0; i < numOutputs; i++)
	{
		output[i] = new Socket(this, i, false, &outputs_l[i]);
	}
}

void Socket::load(JSON& socket_j) {
	linked = socket_j("linked").get<bool>();
}

void NodeTree::save(JSON& nodetree_j) {
	JSON& node_l = nodetree_j("node");
	for(size_t i = 0; i < node.size(); i++)
	{
		Node* n = node[i];
		n->id = i;
		n->save(node_l.at(i));
	}

	//Links are saved seperately to ensure IDs have been reallocated correctly
	JSON& link_l = nodetree_j("link");
	for(size_t i = 0; i < link.size(); i++)
	{
		Link* l = link[i];
		l->id = i;
		l->save(link_l.at(i));
	}
}

void Node::save(JSON& node_j) {
	data.save(node_j);

	JSON& input_l = node_j("input");
	for(size_t i = 0; i < input.size(); i++)
	{
		Socket* s = input[i];
		s->id = i;
		s->save(input_l.at(i));
	}
	JSON& output_l = node_j("output");
	for(size_t i = 0; i < output.size(); i++)
	{
		Socket* s = output[i];
		s->id = i;
		s->save(output_l.at(i));
	}
}

void Socket::save(JSON& socket_j) {
	data.save(socket_j);

	socket_j("linked").set(linked);
}

void Link::save(JSON& link_j) {
	link_j("toNode").set(to->node->id);
	link_j("toSocket").set(to->id);
	link_j("fromNode").set(from->node->id);
	link_j("fromSocket").set(from->id);
}