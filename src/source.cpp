#include <iostream>
#include "networknode.h"

using namespace std;

int main()
{
	/*int(*f2)(int) = [](int x) -> int {return x;};
	Property prop = Property();
	prop.set(f2);
	cout << prop.get<int(*)(int)>()(10);*/
	
	//typedef Network::Neuron Neuron;
	//typedef Network::Synapse Synapse;
	
	//cout << 1 << endl;
	vector<Node*> node;
	node.push_back(new Node());
	node.push_back(new Node());
	node.push_back(new Node(2));
	node[0]->init<Neuron, Synapse>();
	node[1]->init<Neuron, Synapse>();
	node[2]->init<Neuron, Synapse>();

	SimpComp sc = SimpComp(3);
	sc.init<Neuron, Synapse>();

	//cout << 2 << endl;
	vector<Link*> link;
	link.push_back(new Link(node[0]->output, node[2]->input[0]));
	link.push_back(new Link(node[1]->output, node[2]->input[1]));
	
	//cout << 3 << endl;
	node[0]->output->data.getr<Synapse>().set(1.0);
	node[1]->output->data.getr<Synapse>().set(2.0);

	sc.node[0]->output->data.getr<Synapse>().set(1.0);
	sc.node[1]->output->data.getr<Synapse>().set(2.0);
	
	//cout << 4 << endl;
	//node[2]->calc();
	node[2]->data.getr<Neuron>().calc();
	
	sc.node[2]->data.getr<Neuron>().calc();

	//cout << 5 << endl;
	cout << "hello node " << node[2]->output->data.getr<Synapse>().get() << endl;
	cout << "simplicial " << sc.node[2]->output->data.getr<Synapse>().get() << endl;
	int junk;
	cin >> junk;
	return 0;
}