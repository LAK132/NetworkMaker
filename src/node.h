#include <stdlib.h>
#include <vector>
#include "property.h"
#ifndef NODE_H
#define NODE_H

using std::vector;

class Node;
class Link;

class Socket {
	public:
	Node* parent;
	Link* link;
	double weight;
	Property data;
	Socket(Node* n, double w = 1.0);
	~Socket();
};

class Link {
	public:
	Socket* to;
	Socket* from;
	Link(Socket* f, Socket* t) {
		to = t;
		t->link = this;
		from = f;
		f->link = this;
	}
	~Link(){}
};

Socket::Socket(Node* n, double w) {
	parent = n;
	weight = w;
	link = 0;
}
Socket::~Socket() {
	if (link != 0) delete link;
}

class Node {
	public:
	vector<Socket*> input;
	Socket* output;
	
	Node(size_t numInputs = 0) {
		input.resize(numInputs);
		for (auto it = input.begin(); it != input.end(); it++) *it = new Socket(this);
		output = new Socket(this);
	}
	~Node() {
		for (auto it = input.begin(); it != input.end(); it++) delete *it;
		delete output;
	}
	void calc() {
		double temp = 0.0;
		for (auto it = input.begin(); it != input.end(); it++) {
			if ((*it)->link != 0) temp += (*it)->weight * (*it)->link->from->parent->output->data.get<double>();
			else temp += (*it)->weight * (*it)->data.get<double>();
		}
		output->data.set(temp);
	}
};

#endif