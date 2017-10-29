#include <stdlib.h>
#include "property.h"
#ifndef NODE_H
#define NODE_H

class Node;

class Link {
	private:
	public:
	Node* parent;
	double weight;
};

class Node {
	private:
	public:
	size_t numIn;
	Link** input;
	Link output;
	Property data;
	
	Node(size_t numInputs = 0) {
		numIn = numInputs;
		input = (Link**)malloc (numInputs * sizeof (Link*));
		output.parent = this;
	}
	~Node() {
		delete[] input;
	}
	void calc() {
		double temp = 0.0;
		for (size_t i = 0; i < numIn; i++) {
			temp += input[i]->weight * input[i]->parent->data.get<double>();
		}
		data.set(temp);
	}
};

#endif