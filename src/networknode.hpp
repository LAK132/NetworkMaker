#include <vector>
using std::vector;
#include "node.hpp"
#include "json.hpp"
#include "property.hpp"

#ifndef NETWORKNODE_H
#define NETWORKNODE_H

/*class SimpComp
{
public:
	size_t treeID;
	vector<Node*> node;
	vector<Link*> link;
	SimpComp(size_t dimension, size_t tree = 0)
	{
		treeID = tree;
		node.resize(dimension);
		link.resize((dimension * (dimension + 1)) / 2);
		for (size_t n = 0; n < dimension; n++)
		{
			node[n] = new Node(n, n, treeID);
			for (size_t l = 0; l < n; l++)
			{
				link[l] = new Link(node[l]->output, node[n]->input[l]);
			}
		}
	}
	~SimpComp()
	{
		for (auto it = node.begin(); it != node.end(); it++) delete (*it);
	}
	template<typename N, typename S>
	void init(SQL* sql)
	{
		for (auto it = node.begin(); it != node.end(); it++) (*it)->init<N, S>(sql);
	}
};*/

class Synapse : public Socket
{
private:
	double val;
public:
	double weight;
	using Socket::Socket;
	void set(double value);
	double get();
	void loadData(JSON& json);
	void saveData(JSON& json);
};

class Neuron : public Node
{
public:
	using Node::Node;
	void poll();
	void loadData(JSON& json);
	void saveData(JSON& json);
};

#endif