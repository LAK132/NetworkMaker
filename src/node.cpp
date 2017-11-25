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

class DefaultNode : public Node {
    using Node::Node;
    void loadData(JSON& data_j){}
	void saveData(JSON& data_j){}
	void poll(){}
	void draw(bool&){}
};
class DefaultSocket : public Socket {
    using Socket::Socket;
    void loadData(JSON& data_j){}
	void saveData(JSON& data_j){}
	void draw(bool&){}
};

map<string, NodeMaker> MakeNode = {{"", NodeType<DefaultNode>}};
map<string, SocketMaker> MakeSocket = {{"", SocketType<DefaultSocket>}};

NodeGraph::NodeGraph(NodeTree* nt) {
	nodeTree = nt;
}

NodeTree::NodeTree(uint64_t ntid, JSON* json) {
	id = ntid;
    if (json != 0) load(*json);
}

Node::Node(NodeTree* nt, uint64_t nid, JSON* json) {
    nodetree = nt;
    id = nid;
    if (json != 0)
        load(*json);
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
	from->linked = false;
}

Node* NodeTree::addNode(const string& nodeType, JSON* json) {
	Node* rtn = MakeNode[nodeType](this, node.size(), json);
	if(strcmp(rtn->type.c_str(), "")) rtn->type = nodeType;
	node.push_back(rtn);
	return rtn;
}

Node* NodeTree::addNode(JSON* json) {
	return addNode((*json).init<string>("type", ""), json);
}

Link* NodeTree::addLink(Socket* from, Socket* to) {
	if(from->input) return 0;
	Link* rtn = new Link(from, to);
	link.push_back(rtn);
	return rtn;
}

Socket* Node::addSocket(bool isIn, const string& sockType, JSON* json) {
	vector<Socket*>* sock = (isIn ? &input : &output);
	Socket* rtn = MakeSocket[sockType](this, sock->size(), isIn, json);
	if(strcmp(rtn->type.c_str(), "")) rtn->type = sockType;
	sock->push_back(rtn);
	return rtn;
}

Socket* Node::addSocket(bool isIn, JSON* json) {
	return addSocket(isIn, (*json).init<string>("type", ""), json);
}

Socket* Node::addInput(const string& sockType, JSON* json) {
    return addSocket(true, sockType, json);
}

Socket* Node::addInput(JSON* json) {
    return addInput((*json).init<string>("type", ""), json);
}

Socket* Node::addOutput(const string& sockType, JSON* json) {
    return addSocket(false, sockType, json);
}

Socket* Node::addOutput(JSON* json) {
    return addOutput((*json).init<string>("type", ""), json);
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

	uint64_t numNodes = node_l.arrSize();
	node.resize(numNodes);
	for (uint64_t i = 0; i < numNodes; i++)
	{
		JSON& node_j = node_l[i];
		node[i] = MakeNode[node_j.init<string>("type", "")](this, i, &node_j);
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
	type = node_j.init<string>("type", "");
    name = node_j.init<string>("name", "Node " + to_string(id));
	pos.x = node_j("pos").init<float>(0, 0.0f);
	pos.y = node_j("pos").init<float>(1, 0.0f);
	JSON& input_l = node_j("input");
	uint64_t numInputs = input_l.arrSize();
	input.resize(numInputs);
	for (uint64_t i = 0; i < numInputs; i++)
	{
		JSON& input_j = input_l[i];
		input[i] = MakeSocket[input_j.init("type", (string)"")](this, i, true, &input_j);
	}

	JSON& output_l = node_j("output");
	uint64_t numOutputs = output_l.arrSize();
	output.resize(numOutputs);
	for (uint64_t i = 0; i < numOutputs; i++)
	{
		JSON& output_j = output_l[i];
		output[i] = MakeSocket[output_j.init("type", (string)"")](this, i, false, &output_j);
	}
	loadData(node_j("data"));
}

void Socket::load(JSON& socket_j) {
	type = socket_j.init<string>("type", "");
    name = socket_j.init<string>("name", "Socket " + to_string(id));
	linked = socket_j.init<bool>("linked", false);
	loadData(socket_j("data"));
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

bool NodeGraph::render(ImDrawList* drawList, ImVec2 offset) {
	bool updt = false;
	ImGui::BeginGroup();
	ImGui::Text("Node Graph");
	offset = ImGui::GetCursorScreenPos();
	ImGui::BeginChild("scrolling_region", ImVec2(0,0), true, ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);

	if (showGrid)
    {
        ImU32 GRID_COLOR = ImColor(200,200,200,40);
        float GRID_SZ = 64.0f;
        ImVec2 win_pos = offset;
        ImVec2 canvas_sz = ImGui::GetWindowSize();
        for (float x = fmodf(-scrolling.x,GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
            drawList->AddLine(ImVec2(x,0.0f)+win_pos, ImVec2(x,canvas_sz.y)+win_pos, GRID_COLOR);
        for (float y = fmodf(-scrolling.y,GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
            drawList->AddLine(ImVec2(0.0f,y)+win_pos, ImVec2(canvas_sz.x,y)+win_pos, GRID_COLOR);
    }

	offset = offset-scrolling;

	updt |= nodeTree->render(drawList, offset);

	if(ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(1, 0.0f)) {
		scrolling -= ImGui::GetIO().MouseDelta;
	}

	ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::EndGroup();
	return updt;
}

bool NodeTree::render(ImDrawList* drawList, ImVec2 offset) {
    bool updt = false;
	drawList->ChannelsSplit(2);

	drawList->ChannelsSetCurrent(1); //Foreground
	for(auto it = node.begin(), end = node.end(); it != end; ++it){
		updt |= (*it)->render(drawList, offset);
	}

	drawList->ChannelsSetCurrent(0); //Background
	for(auto it = link.begin(), end = link.end(); it != end; ++it){
		updt |= (*it)->render(drawList, offset);
	}

	/*const string name = "NodeTree" + to_string(id);
    if(ImGui::TreeNode("NodeTree", "NodeTree%d", id)) {
		drawList->ChannelsSetCurrent(1); //Foreground
        for(auto it = node.begin(), end = node.end(); it != end; ++it){
            updt |= (*it)->render(drawList, offset);
		}

		drawList->ChannelsSetCurrent(0); //Background
        for(auto it = link.begin(), end = link.end(); it != end; ++it){
            updt |= (*it)->render(drawList, offset);
        }
        ImGui::TreePop();
    }*/
	drawList->ChannelsMerge();
    return updt;
}

bool Node::render(ImDrawList* drawList, ImVec2 offset) {
	bool updt = false;
	ImGui::PushID(id);
	offset = offset + pos;
	//const ImVec2 currPos = pos + offset;
    bool oldAnyActive = ImGui::IsAnyItemActive();
	ImGui::SetCursorScreenPos(offset + padding);

	ImGui::BeginGroup(); // Lock horizontal position
	ImGui::Text(name.c_str());
	//ImGui::Separator();
    draw(updt);
    for(auto it = input.begin(), end = input.end(); it != end; ++it) {
        updt |= (*it)->render(drawList, offset);
    }
    for(auto it = output.begin(), end = output.end(); it != end; ++it) {
        updt |= (*it)->render(drawList, offset);
    }
	ImGui::EndGroup();

	bool nodeWidgetsActive = (!oldAnyActive && ImGui::IsAnyItemActive());
	size = ImGui::GetItemRectSize() + padding + padding;
	ImVec2 farPos = offset + size;

	drawList->ChannelsSetCurrent(0);
	ImGui::SetCursorScreenPos(offset);
	ImGui::InvisibleButton("node", size);
	if(ImGui::IsItemHovered()) {

	}
	bool nodeMoveActive = ImGui::IsItemActive();
	if(nodeMoveActive || nodeWidgetsActive) {

	}
	if(nodeMoveActive && ImGui::IsMouseDragging(0)) {
		pos += ImGui::GetIO().MouseDelta;
	}

	drawList->AddRectFilled(offset, farPos, ImColor(60, 60, 60), 4.0f);
	drawList->AddRect(offset, farPos, ImColor(100, 100, 100), 4.0f);

	ImGui::PopID();
    return updt;
}

bool Socket::render(ImDrawList* drawList, ImVec2 offset) {
	bool updt = false;

    ImGui::BeginGroup();
    pos = ImGui::GetCursorScreenPos();
    //ImGui::Button(((input?"S##i":"S##o") + to_string(id)).c_str());
    //ImGui::SameLine();
    //ImGui::BeginGroup();
    draw(updt);
    //ImGui::EndGroup();
    ImGui::EndGroup();

    float sockHeight = ImGui::GetItemRectSize().y/2;
    pos.y += sockHeight;
    pos.x = offset.x + (input ? 0 : node->size.x);

    drawList->ChannelsSetCurrent(1);
    drawList->AddCircleFilled(pos, 5.f, ImColor(255, 255, 255, 127), 8);

    return updt;
}

bool Link::render(ImDrawList* drawList, ImVec2 offset) {
    bool updt = false;
	ImVec2 p1 = from->pos;// + offset;
	ImVec2 p2 = to->pos;// + offset;
	drawList->AddBezierCurve(p1, p1+ImVec2(50, 0), p2+ImVec2(-50, 0), p2, ImColor(200, 200, 100), 3.0f);
	/*if(ImGui::TreeNode("Link", "Link%d", id)) {
        updt |= from->render(drawList, offset);
        updt |= to->render(drawList, offset);
        ImGui::TreePop();
    }*/
    return updt;
}
