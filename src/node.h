#include <stdlib.h>
#include <vector>
#include "property.h"
#ifndef NODE_H
#define NODE_H

using std::vector;

class Node;

class Socket {
	private:
	public:
	Node* parent;
	double weight;
};

class Link {
	public:
	Socket* to;
	Socket* from;
	Link(Socket* t, Socket* f) {
		to = t;
		from = f;
	}
};

class Node {
	private:
	public:
	vector<Socket*> input;
	Socket* output;
	Property data;
	
	Node(size_t numInputs = 0) {
		input.resize(numInputs);
		for (auto it = input.begin(); it != input.end(); it++) *it = new Socket();
		output = new Socket();
		output->parent = this;
	}
	~Node() {
		for (auto it = input.begin(); it != input.end(); it++) delete *it;
		delete output;
	}
	void calc() {
		double temp = 0.0;
		for (size_t i = 0; i < input.size(); i++) {
			temp += input[i]->weight * input[i]->parent->data.get<double>();
		}
		data.set(temp);
	}
};

#endif