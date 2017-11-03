#include <vector>
using std::vector;
#include "node.h"

class SimpComp
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
};

class Synapse
{
private:
	double val;
public:
	double weight;
	Socket *sock;
	Synapse(Socket* s, map<string, vector<string>>* d)
	{
		sock = s;
		weight = 1.0;
	}
	void set(double value)
	{
		val = value;
	}
	double get()			// Socket* sock)
	{
		if (sock->link == 0 || sock->link->from == sock)
		{
			return val * weight;
		}
		else
		{
			Socket *other = sock->link->from->parent->output;
			double rtn = other->data.getr<Synapse>().get();
			return rtn * weight;
		}
	}
};

class Neuron
{
public:
	Node * node;
	Neuron(Node* n, map<string, vector<string>>* d)
	{
		node = n;
	}
	void calc()
	{
		double temp = 0.0;
		for (auto it = node->input.begin(); it != node->input.end(); it++)
		{
			// printf("temp %d\n",(int)temp);
			temp += (*it)->data.getr<Synapse>().get();	// *it);
		}
		node->output->data.getr<Synapse>().set(temp);
	}
};