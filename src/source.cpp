#include <iostream>
#include "node.h"

using namespace std;

int main()
{
	Node node = Node();
	node.output.weight = 1;
	Node node2 = Node();
	node2.output.weight = 1;
	Node node3 = Node(2);
	node.data.set(1.0);
	node2.data.set(2.0);
	node3.input[0] = &node.output;
	node3.input[1] = &node2.output;
	node3.calc();
	cout << "hello node " << node3.data.get<double>() << endl;
	int junk;
	cin >> junk;
	return 0;
}