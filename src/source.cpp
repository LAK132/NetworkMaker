//Node backend
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "networknode.hpp"
#include "node.hpp"
#include "json.hpp"
#include "property.hpp"

//Graphics
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

//https://gist.github.com/ocornut/7e9b3ec566a333d725d4

int main(int argc, char **argv)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Dear ImGUI + SFML");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    // Load a sprite to display
    /*sf::Texture texture;
    if (!texture.loadFromFile("cb.bmp"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);*/

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


	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);


    bool testWindow;
	sf::Clock deltaClock;
	// Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::ShowTestWindow(&testWindow);


        ImGui::Begin("Hello World!");
            n->poll();
            nt.render();
            /*if(ImGui::Button("Look at this pretty button")){
                window.close();
            }*/
        ImGui::End();

        window.clear();
        //window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
