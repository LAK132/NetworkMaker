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
	//cout << prop.get<int(*)(int)>()(10);*/
	
	//typedef Network::Neuron Neuron;
	//typedef Network::Synapse Synapse;

	JSON json = JSON();
	
	//cout << "1\n";
	
	cin >> json;

	//cout << "1 A\n";

	Neuron n;
	Synapse s;

	JSON& nodetree_j = json("nodetree").at(0);
	JSON& node_l = nodetree_j("node");

	//node_l[0]("data").set(n);
	//node_l[0]("input")[0]("data").set(s);
	node_l[0]("output")[0]("linked").set(true);
	//node_l[0]("output")[0]("data").set(s);

	//cout << "1 A 1\n";
	
	//cout << json("nodetree")[0]("node")[0]("socket")[0]("input").get<bool>() << endl;
	//cout << json("nodetree")[0]("node")[0]("socket")[1]("input").get<bool>() << endl;

	//cout << "1 B\n";

	//node_l[1]("data").set(n);
	//node_l[1]("input")[0]("data").set(s);
	node_l[1]("output")[0]("input").set(false);
	node_l[1]("output")[0]("linked").set(true);
	//node_l[1]("output")[0]("data").set(s);

	//cout << "1 C\n";
	
	//node_l[2]("data").set(n);
	node_l[2]("input")[0]("linked").set(true);
	//node_l[2]("input")[0]("data").set(s);
	node_l[2]("input")[1]("linked").set(true);
	//node_l[2]("input")[1]("data").set(s);
	//node_l[2]("output")[0]("data").set(s);
	
	//cout << "1 D\n";

	JSON& link_l = nodetree_j("link");
		
	link_l[0]("fromNode").set((uint64_t)0);
	link_l[0]("fromSocket").set((uint64_t)0);
	link_l[0]("toNode").set((uint64_t)2);
	link_l[0]("toSocket").set((uint64_t)0);

	//cout << "1 E\n";

	link_l[1]("fromNode").set((uint64_t)1);
	link_l[1]("fromSocket").set((uint64_t)0);
	link_l[1]("toNode").set((uint64_t)2);
	link_l[1]("toSocket").set((uint64_t)1);
	
	//cout << "2\n";

	//cout << json;

	//cout << "2 A\n";
	////cout << 1 << endl;
	//vector<Node*> node;
	//node.push_back(new Node(0, 0));
	//node.push_back(new Node(0, 1));	
	//node.push_back(new Node(2, 2));

	//cout << "3\n";

	NodeTree nt = NodeTree(0, &nodetree_j);

	//cout << "4\n";
	
	nt.init<Neuron, Synapse>(nodetree_j);

	//cout << "5\n";

	nt.id = 0;
	nt.save(nodetree_j);

	//cout << "5\n";

	cout << json;

	//node[0]->init<Neuron, Synapse>(file);
	//node[1]->init<Neuron, Synapse>(file);
	//node[2]->init<Neuron, Synapse>(file);
	
	//cout << "5\n";

	//SimpComp sc = SimpComp(3, 0);
	//sc.init<Neuron, Synapse>(file);
	
	//cout << "6\n";

	////cout << "simplicial complex " << sc.node[2]->output->data.getr<Synapse>().get() << endl;
	
	//cout << "6\n";
	
	////cout << 3 << endl;
	//node[0]->output->data.getr<Synapse>().set(1.0);
	//node[1]->output->data.getr<Synapse>().set(2.0);

	//sc.node[0]->output->data.getr<Synapse>().set(1.0);
	//sc.node[1]->output->data.getr<Synapse>().set(2.0);
	
	////cout << 4 << endl;
	//node[2]->calc();
	//node[2]->data.getr<Neuron>().calc();
	
	//sc.node[2]->data.getr<Neuron>().calc();

	////cout << 5 << endl;
	////cout << "hello node " << node[2]->output->data.getr<Synapse>().get() << endl;
	////cout << "simplicial " << sc.node[2]->output->data.getr<Synapse>().get() << endl;
	////cout << "node " << nt.node[2]->output->data.getr<Synapse>().get() << endl;
	int junk;
	cin >> junk;
	return 0;
}