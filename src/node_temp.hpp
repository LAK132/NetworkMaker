template<typename N, typename S>
void NodeTree::init(JSON* json)
{
	for(auto it = node.begin(); it != node.end(); it++) (*it)->init<N, S>(json);
}

template<typename N, typename S>
void Node::init(JSON* json)
{
	N n = (*json)("nodetree")[nodetree->id]("node")[id]("data").get<N>();
	data.init<N>(this, n);

	for(auto it = input.begin(); it != input.end(); it++) (*it)->init<S>(json);
	for(auto it = output.begin(); it != output.end(); it++) (*it)->init<S>(json);
}

template<typename S>
void Socket::init(JSON* json)
{
	S s = (*json)("nodetree")[node->nodetree->id]("node")[node->id]((input ? "input" : "output"))[id]("data").get<S>();
	data.init<S>(this, s);
}