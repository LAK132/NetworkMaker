template<typename N, typename S>
void NodeTree::init(JSON& nodetree_j)
{
	JSON& node_l = nodetree_j("node");
	for(size_t i = 0; i < node_l.size(); i++)
	{
		node[i]->init<N, S>(node_l[i]);
	}
}

template<typename N, typename S>
void Node::init(JSON& node_j)
{
	N n = N(this);
	n.load(node_j("data"));

	JSON& input_l = node_j("input");
	for(size_t i = 0; i < input_l.size(); i++)
	{
		input[i]->init<S>(input_l[i]);
	}

	JSON& output_l = node_j("output");
	for(size_t i = 0; i < output_l.size(); i++)
	{
		output[i]->init<S>(output_l[i]);
	}
}

template<typename S>
void Socket::init(JSON& socket_j)
{
	S s = S(this);
	s.load(socket_j("data"));
}