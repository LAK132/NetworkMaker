//Node backend
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include "networknode.hpp"
#include "node.hpp"
#include "json.hpp"
#include "property.hpp"

using namespace std;

//https://gist.github.com/ocornut/7e9b3ec566a333d725d4

int main(int argc, char **argv)
{
	JSON json = JSON();

	JSON& nodetree_j = json("nodetree").at(0);
	JSON& node_l = nodetree_j("node");

	MakeNode.emplace("neuron", NodeType<Neuron>);
	MakeSocket.emplace("synapse", SocketType<Synapse>);

	NodeTree nt = NodeTree(0, 0);

	nt.addNode("neuron");
	nt.addNode("neuron");
	nt.addNode("neuron");
	nt.addNode("neuron");

	nt.node[0]->addInput("synapse");

	nt.node[0]->addOutput("synapse");
	nt.node[1]->addInput("synapse");
	nt.node[0]->output[0]->addLink(nt.node[1]->input[0]);

	nt.node[1]->addOutput("synapse");
	nt.node[2]->addInput("synapse");
	nt.node[1]->output[0]->addLink(nt.node[2]->input[0]);

	nt.node[2]->addOutput("synapse");
	nt.node[3]->addInput("synapse");
	nt.node[2]->output[0]->addLink(nt.node[3]->input[0]);

	nt.node[3]->addOutput("synapse");

	Synapse* s;
	for(auto nit = nt.node.begin(); nit != nt.node.end(); nit++)
	{
		for(auto sit = (*nit)->input.begin(); sit != (*nit)->input.end(); sit++)
		{
			s = (Synapse*)(*sit);
			s->weight = 1.5;
			s->set(1.0);
		}
		for(auto sit = (*nit)->output.begin(); sit != (*nit)->output.end(); sit++)
		{
			s = (Synapse*)(*sit);
			s->weight = 0.0;
			s->set(1.0);
		}
	}

	Neuron* n = (Neuron*)nt.node[3];

	n->poll();

	nt.id = 0;
	nt.save(nodetree_j);

	NodeGraph ng = NodeGraph(&nt);

    nt.poll();

	nt.id = 0;
	nt.save(nodetree_j);

	cout << json << flush;
}
