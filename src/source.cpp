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

void errorModal()
{
    if(ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Error");
        if (ImGui::Button("OK", ImVec2(120,0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void openButton(string& fileName, NodeTree& nt, JSON& json)
{
    if(ImGui::Button("Open"))
    {
        ImGui::OpenPopup("OpenPopup");
    }
    if(ImGui::BeginPopupModal("OpenPopup", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::Button("OK", ImVec2(120,0)))
        {
            if(fileName != "")
            {
                ifstream file;
                file.open(fileName.c_str());
                if(file.is_open())
                {
                    file >> json;
                    nt.load(json("nodetree").at(0));
                    file.close();
                }
                //else ImGui::OpenPopup("Error");
            }
            //else ImGui::OpenPopup("Error");
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120,0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void saveAsButton(string& fileName, NodeTree& nt, JSON& json)
{
    if(ImGui::Button("Save As"))
    {
        ImGui::OpenPopup("SavePopup");
    }
    if(ImGui::BeginPopupModal("SavePopup", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::Button("OK", ImVec2(120,0))) {
            if(fileName != "")
            {
                ofstream file;
                file.open(fileName.c_str(), ofstream::trunc);
                if(file.is_open())
                {
                    nt.save(json("nodetree").at(0));
                    file << json;
                    file.close();
                }
                //else ImGui::OpenPopup("Error");
            }
            //else ImGui::OpenPopup("Error");
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120,0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void mainMenu(NodeTree& nt, JSON& json)
{
    if(ImGui::BeginMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            string fileName = "demo/demo1.json";
            saveAsButton(fileName, nt, json);
            openButton(fileName, nt, json);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

int main(int argc, char **argv)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Dear ImGUI + SFML");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

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

	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);

	NodeGraph ng = NodeGraph(&nt);

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

        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_MenuBar;
        ImGui::Begin("Hello World!", NULL, windowFlags);
            mainMenu(nt, json);
            if(ImGui::Button("Save")) {
                cout << "save" << endl;
                nt.id = 0;
                nt.save(nodetree_j);
            }
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            if(ng.render(drawList)){
                nt.poll();
                //((Neuron*)nt.node[3])->poll();
            }
            /*if(ImGui::Button("Look at this pretty button")){
                window.close();
            }*/
        ImGui::End();

        window.clear();
        //window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

	nt.id = 0;
	nt.save(nodetree_j);

	cout << json << flush;

    ImGui::SFML::Shutdown();
}
