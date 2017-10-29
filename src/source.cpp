#include <iostream>

using namespace std;

typedef struct nodedata {
	double output;
} nodedata_t;

typedef struct node {
	int posX;
	int posY;
	nodedata_t data;
} node_t;

int main()
{
	node_t node;
	node.posX = 10;
	cout << "hello node " << node.posX << endl;
	int junk;
	cin >> junk;
	return 0;
}