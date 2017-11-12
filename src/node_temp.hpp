template<typename N, typename S>
void NodeTree::init(JSON& nodetree_j)
{
	JSON& node_l = nodetree_j("node");
	for(size_t i = 0; i < node.size(); i++)
	{
		node[i]->init<N, S>(node_l.at(i));
	}
}

template<typename N, typename S>
void Node::init(JSON& node_j)
{
	data = new N(this);
	((N*)data)->load(node_j("data"));

	JSON& input_l = node_j("input");
	for(size_t i = 0; i < input.size(); i++)
	{
		input[i]->init<S>(input_l.at(i));
	}

	JSON& output_l = node_j("output");
	for(size_t i = 0; i < output.size(); i++)
	{
		output[i]->init<S>(output_l.at(i));
	}
}

template<typename S>
void Socket::init(JSON& socket_j)
{
	data = new S(this);
	((S*)data)->load(socket_j("data"));
}