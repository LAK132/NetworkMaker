#include <stdlib.h>
#include "property.h"
#ifndef NODE_H
#define NODE_H

class Node;

class Link {
	private:
	public:
	Link(){}
	~Link();
};

class Node {
	private:
	size_t numLinks;
	Link* link;
	public:
	Node(size_t numInputs = 0) {
		numLinks = numInputs;
		link = (Link*)malloc (numInputs * sizeof (Link));
	}
	~Node() {
		delete[] link;
	}
};

#endif