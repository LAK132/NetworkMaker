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

class Synapse : public Data
{
private:
	double val;
public:
	double weight;
	Socket *sock;
	Synapse();
	Synapse(Socket* s);
	void set(double value);
	double get();
	void load(JSON& json);
	void save(JSON& json);
};

class Neuron : public Data
{
public:
	Node* node;
	Neuron();
	Neuron(Node* n);
	void calc();
	void load(JSON& json);
	void save(JSON& json);
};

#endif