// Copyright (c) 2018 Lucas Kleiss (LAK132)
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <string>
#include <vector>
#include <unordered_map>
#include <imgui.h>

#ifndef NODEGRAPH_H
#define NODEGRAPH_H

namespace lak
{
    using std::vector;
    using std::string;
    using std::unordered_map;

    struct socket_t
    {
        bool input = true;
        ImColor color = ImColor(255, 255, 255, 255);
        ImVec2 offset = ImVec2(0, 0);
    };

    struct node_t
    {
        ImVec2 pos = ImVec2(0, 0);
        ImVec2 size = ImVec2(0, 0);
        unordered_map<string, socket_t> sockets;
    };

    struct graph_t
    {
        ImVec2 scroll = ImVec2(0, 0);
        bool draggingLink = false;
        unordered_map<string, node_t> nodes;
    };

    struct Graph
    {
        ImDrawList *drawList;
        graph_t &graph;

        bool showGrid;
        ImVec2 pos = ImVec2(0, 0);

        bool rtn;
        operator bool();
        Graph(const char *ID, graph_t &_graph, bool _showGrid = true);
        ~Graph();
    };

    struct Node
    {
        Graph &graph;
        node_t &node;

        ImVec2 padding;
        ImVec2 pos;
        bool oldAnyActive;

        bool rtn;
        operator bool();
        Node(string ID, Graph &_graph, ImVec2 _padding = ImVec2(8.0f, 8.0f));
        ~Node();
    };

    struct Socket
    {
        Node &node;
        socket_t &socket;

        ImVec2 pos;

        bool rtn;
        operator bool();
        Socket(string ID, Node &_node);
        ~Socket();
    };
}

#endif // NODEGRAPH_H