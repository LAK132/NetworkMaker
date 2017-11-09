

template<typename N, typename S>
void NodeTree::init(JSON* json)
{
	for(auto it = node.begin(); it != node.end(); it++) (*it)->init<N, S>(json);
}

template<typename N, typename S>
void Node::init(JSON* json)
{
	N n;
	memcpy(&n, (*json)("nodetree")[nodetree->id]("node")[id]("data")->getp<N>(), sizeof(N));
	data.init<N>(this, n);

	for(auto it = input.begin(); it != input.end(); it++) (*it)->init<S>(json);
	for(auto it = output.begin(); it != output.end(); it++) (*it)->init<S>(json);
}

template<typename S>
void Socket::init(JSON* json)
{
	S s;
	memcpy(&s, (*json)("nodetree")[node->nodetree->id]("node")[node->id]("socket")[id]("data")->getp<S>(), sizeof(S));
	data.init<S>(this, s);
}

template<typename N, typename S>
void NodeTree::save(JSON* json)
{
	
}

template<typename N, typename S>
void Node::save(JSON* json)
{
	
}

template<typename S>
void Socket::save(JSON* json)
{

}