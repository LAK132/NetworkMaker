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

void Node::loadData(JSON& data_j){}
void Node::saveData(JSON& data_j){}
Node* Node::poll(){return nullptr;}

void Socket::loadData(JSON& data_j){}
void Socket::saveData(JSON& data_j){}

map<string, NodeMaker> MakeNode = {{"", NodeType<Node>}};
map<string, SocketMaker> MakeSocket = {{"", SocketType<Socket>}};

NodeGraph::NodeGraph(NodeTree* nt) {
	nodeTree = nt;
}

NodeTree::NodeTree(uint64_t ntid, JSON* json) {
	id = ntid;
    if (json != 0) load(*json);
}

void NodeTree::poll() {
    for(auto it = node.begin(); it != node.end(); it++) {
        if ((*it)->polled) continue;
        vector<Node*> prev;
        Node* first = (*it);
        Node* next;
        Node* curr = first;
        do {
            next = curr->poll();
            if (curr->polled) {
                if (prev.size() > 0) {
                    curr = prev.back();
                    prev.pop_back();
                }
                else {
                    curr = nullptr;
                }
            }
            else {
                prev.push_back(curr);
                curr = next;
            }
        } while (curr != nullptr && curr != first);
    }
    for(auto it = node.begin(); it != node.end(); it++) (*it)->polled = false;
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

NodeTree::~NodeTree() {
	for (auto it = node.begin(); it != node.end(); it++) delete *it;
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

Link::~Link() {
	to->linked = false;
	to->link = nullptr;
	from->linked = false;
	from->link = nullptr;
	to->node->nodetree->_removeLink(this);
}

Node* NodeTree::addNode(const string& nodeType, JSON* json) {
	Node* rtn = MakeNode[nodeType](this, node.size(), json);
	rtn->type = nodeType;
	node.push_back(rtn);
	return rtn;
}

Node* NodeTree::addNode(JSON* json) {
	return addNode((*json).init<string>("type", (string)""), json);
}

Link* NodeTree::addLink(Socket* from, Socket* to) {
	if(from->input) return 0;
	Link* rtn = new Link(from, to);
	link.push_back(rtn);
	return rtn;
}

void NodeTree::_removeLink(Link* lnk) {
    auto it = link.begin();
	size_t id = 0;
    for (auto en = link.end(); *it != lnk && it != en; it++) {
        (*it)->id = id++;
    }
    link.erase(it);
    for (auto en = link.end(); it != en; it++) {
        (*it)->id = id++;
    }
}

void NodeTree::removeLink(Link* lnk) {
    delete lnk;
}

Socket* Node::addSocket(bool isIn, const string& sockType, JSON* json) {
	vector<Socket*>* sock = (isIn ? &input : &output);
	Socket* rtn = MakeSocket[sockType](this, sock->size(), isIn, json);
	rtn->type = sockType;
	sock->push_back(rtn);
	return rtn;
}

Socket* Node::addSocket(bool isIn, JSON* json) {
	return addSocket(isIn, (*json).init<string>("type", (string)""), json);
}

Socket* Node::addInput(const string& sockType, JSON* json) {
    return addSocket(true, sockType, json);
}

Socket* Node::addInput(JSON* json) {
    return addInput((*json).init<string>("type", (string)""), json);
}

Socket* Node::addOutput(const string& sockType, JSON* json) {
    return addSocket(false, sockType, json);
}

Socket* Node::addOutput(JSON* json) {
    return addOutput((*json).init<string>("type", (string)""), json);
}

Link* Socket::addLink(Socket* other) {
	if(input == other->input) return 0;
	if(input)
	{
		return node->nodetree->addLink(other, this);
	}
	else
	{
		return node->nodetree->addLink(this, other);
	}
}

void NodeTree::load(JSON& nodetree_j) {
	JSON& node_l = nodetree_j("node");
    name = nodetree_j.init<string>("name", "NodeTree " + to_string(id));

    for (auto it = node.begin(); it != node.end(); it++) delete *it; //node.resize does not handle deletion of objects

    map<string, NodeMaker>& temp = MakeNode;

	uint64_t numNodes = node_l.arrSize();
	node.resize(numNodes);
	for (uint64_t i = 0; i < numNodes; i++)
	{
		JSON& node_j = node_l[i];
		node[i] = MakeNode[node_j.init<string>("type", (string)"")](this, i, &node_j);
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
	type = node_j.init<string>("type", (string)"");
    name = node_j.init<string>("name", "Node " + to_string(id));
	pos.x = node_j("pos").init<float>(0, 0.0f);
	pos.y = node_j("pos").init<float>(1, 0.0f);
	JSON& input_l = node_j("input");
	uint64_t numInputs = input_l.arrSize();
	input.resize(numInputs);
	for (uint64_t i = 0; i < numInputs; i++)
	{
		JSON& input_j = input_l[i];
		//string str;
		//str = input_j.init<string>("type", (string)"");
		//auto func = MakeSocket[str];
		//input[i] = func(this, i, true, &input_j);
		input[i] = MakeSocket[input_j.init<string>("type", (string)"")](this, i, true, &input_j);
	}

	JSON& output_l = node_j("output");
	uint64_t numOutputs = output_l.arrSize();
	output.resize(numOutputs);
	for (uint64_t i = 0; i < numOutputs; i++)
	{
		JSON& output_j = output_l[i];
		output[i] = MakeSocket[output_j.init<string>("type", (string)"")](this, i, false, &output_j);
	}
	loadData(node_j("data"));
}

void Socket::load(JSON& socket_j) {
	type = socket_j.init<string>("type", (string)"");
    name = socket_j.init<string>("name", "Socket " + to_string(id));
	linked = socket_j.init<bool>("linked", false);
	JSON j = socket_j("data");
	loadData(j);
}

void Link::load(JSON& link_j) {}

void NodeTree::save(JSON& nodetree_j) {
	nodetree_j.clear();
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
	node_j("type").set(type);
	node_j("pos").at(0).set(pos.x);
	node_j("pos").at(1).set(pos.y);
	saveData(node_j("data"));
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
	socket_j("type").set(type);
	socket_j("linked").set(linked);
	saveData(socket_j("data"));
}

void Link::save(JSON& link_j) {
	link_j("toNode").set(to->node->id);
	link_j("toSocket").set(to->id);
	link_j("fromNode").set(from->node->id);
	link_j("fromSocket").set(from->id);
}
