#include <iostream>
#include "node.h"

using namespace std;

int main()
{
	Node node = Node();
	Node node2 = Node();
	Node node3 = Node(2);
	
	Link a = Link(node.output, node3.input[0]);
	Link b = Link(node2.output, node3.input[1]);
	
	node.output->data.set(1.0);
	node2.output->data.set(2.0);
	
	node3.calc();
	cout << "hello node " << node3.output->data.get<double>() << endl;
	int junk;
	cin >> junk;
	return 0;
}