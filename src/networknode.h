#include <vector>
using std::vector;
#include "node.h"
#ifndef NETWORK_NODES_H
#define NETWORK_NODES_H

class SimpComp
{
public:
	vector<Node*> node;
	vector<Link*> link;
	SimpComp(size_t dimension)
	{
		node.resize(dimension);
		link.resize((dimension * (dimension + 1)) / 2);
		for (size_t n = 0; n < dimension; n++)
		{
			node[n] = new Node(n);
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
	void init()
	{
		for (auto it = node.begin(); it != node.end(); it++) (*it)->init<N, S>();
	}
};

class Synapse
{
private:
	double val;
public:
	double weight;
	Socket *sock;
	Synapse(Socket * s)
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
	Neuron(Node * n)
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

#endif