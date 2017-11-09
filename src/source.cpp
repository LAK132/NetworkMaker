#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "networknode.hpp"
#include "node.hpp"
#include "json.hpp"
#include "property.hpp"

using namespace std;

int main()
{
	/*int(*f2)(int) = [](int x) -> int {return x;};
	Property prop = Property();
	prop.set(f2);
	cout << prop.get<int(*)(int)>()(10);*/
	
	//typedef Network::Neuron Neuron;
	//typedef Network::Synapse Synapse;
	
	cout << "1\n";
	
	cout << "2\n";

	//cout << 1 << endl;
	//vector<Node*> node;
	//node.push_back(new Node(0, 0));
	//node.push_back(new Node(0, 1));	
	//node.push_back(new Node(2, 2));

	JSON json;

	NodeTree nt = NodeTree(0, &json);
	
	cout << "3\n";

	nt.init<Neuron, Synapse>(&json);
	//node[0]->init<Neuron, Synapse>(file);
	//node[1]->init<Neuron, Synapse>(file);
	//node[2]->init<Neuron, Synapse>(file);
	
	cout << "4\n";

	//SimpComp sc = SimpComp(3, 0);
	//sc.init<Neuron, Synapse>(file);
	
	cout << "5\n";

	//cout << "simplicial complex " << sc.node[2]->output->data.getr<Synapse>().get() << endl;
	
	cout << "6\n";
	
	//cout << 3 << endl;
	//node[0]->output->data.getr<Synapse>().set(1.0);
	//node[1]->output->data.getr<Synapse>().set(2.0);

	//sc.node[0]->output->data.getr<Synapse>().set(1.0);
	//sc.node[1]->output->data.getr<Synapse>().set(2.0);
	
	//cout << 4 << endl;
	//node[2]->calc();
	//node[2]->data.getr<Neuron>().calc();
	
	//sc.node[2]->data.getr<Neuron>().calc();

	//cout << 5 << endl;
	//cout << "hello node " << node[2]->output->data.getr<Synapse>().get() << endl;
	//cout << "simplicial " << sc.node[2]->output->data.getr<Synapse>().get() << endl;
	//cout << "node " << nt.node[2]->output->data.getr<Synapse>().get() << endl;
	int junk;
	cin >> junk;
	return 0;
}