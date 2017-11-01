#include <iostream>
#include "networknode.h"

using namespace std;

int main()
{
	SimpComp sc = SimpComp(3);
	sc.init<Neuron, Synapse>();
	sc.node[0]->output->data.getr<Synapse>().set(1.0);
	sc.node[1]->output->data.getr<Synapse>().set(2.0);
	
	sc.node[2]->data.getr<Neuron>().calc();

	cout << "simplicial complex " << sc.node[2]->output->data.getr<Synapse>().get() << endl;
	
	
	int junk;
	cin >> junk;
	return 0;
}