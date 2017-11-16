#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
//#include <gtk/gtk.h>
#include "networknode.hpp"
#include "node.hpp"
#include "json.hpp"
#include "property.hpp"

using namespace std;

/*static void activate (GtkApplication* app, gpointer user_data) {
  GtkWidget *window;
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_widget_show_all (window);
}*/

int main(int argc, char **argv)
{
	/*GtkApplication *app;
	int status;

	app = gtk_application_new ("com.kicodora.networkmaker", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);*/

	JSON json = JSON();
	
	//cin >> json;

	JSON& nodetree_j = json("nodetree").at(0);
	JSON& node_l = nodetree_j("node");

	/*//node_l[0]("data").set(n);
	//node_l[0]("input")[0]("data").set(s);
	node_l[0]("output")[0]("linked").set(true);
	//node_l[0]("output")[0]("data").set(s);

	//cout << "1 A 1\n";
	
	//cout << json("nodetree")[0]("node")[0]("socket")[0]("input").get<bool>() << endl;
	//cout << json("nodetree")[0]("node")[0]("socket")[1]("input").get<bool>() << endl;

	//cout << "1 B\n";

	//node_l[1]("data").set(n);
	//node_l[1]("input")[0]("data").set(s);
	//node_l[1]("output")[0]("input").set(false);
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
	link_l[1]("toSocket").set((uint64_t)1);*/
	
	//cout << "2\n";

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

	cout << json << flush;

	int junk;
	cin >> junk;
	return 0;
}