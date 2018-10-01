// Copyright (c) 2018 Lucas Kleiss (LAK132)
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "nodegraph.h"

inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

namespace lak
{
    Graph::operator bool()
    {
        return rtn;
    }

    Graph::Graph(const char *ID, graph_t &_graph, bool _showGrid)
    : graph(_graph), showGrid(_showGrid)
    {
        ImGui::BeginGroup();
        pos = ImGui::GetCursorScreenPos();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        rtn = ImGui::BeginChild(ID, ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
        drawList = ImGui::GetWindowDrawList();
        drawList->ChannelsSplit(4);
        drawList->ChannelsSetCurrent(0);
        ImGui::PushItemWidth(120.0f);
        ImVec2 canvasSize = ImGui::GetWindowSize();
        if (showGrid)
        {
            ImU32 gridColor = ImColor(255, 255, 255, 128);
            float gridSize = 64.0f;
            for (float x = fmodf(-graph.scroll.x, gridSize); x < canvasSize.x; x+= gridSize)
                drawList->AddLine(ImVec2(x, 0.0f) + pos, ImVec2(x, canvasSize.y) + pos, gridColor);
            for (float y = fmodf(-graph.scroll.y, gridSize); y < canvasSize.y; y+= gridSize)
                drawList->AddLine(ImVec2(0.0f, y) + pos, ImVec2(canvasSize.x, y) + pos, gridColor);
        }
        pos = pos - graph.scroll;
        if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
            graph.scroll = graph.scroll - ImGui::GetIO().MouseDelta;
        
        drawList->ChannelsSetCurrent(1); // background

        if (graph.draggingLink)
        {

        }
    }

    Graph::~Graph()
    {
        ImGui::PopItemWidth();
        ImGui::EndChild();
        ImGui::PopStyleVar(2);
        ImGui::EndGroup();
        drawList->ChannelsMerge();
    }

    Node::operator bool()
    {
        return rtn;
    }

    Node::Node(string ID, Graph &_graph, ImVec2 _padding)
    : graph(_graph), node(_graph.graph.nodes[ID]), padding(_padding)
    {
        graph.drawList->ChannelsSetCurrent(2); // background
        rtn = true;
        pos = graph.pos + node.pos;
        ImGui::PushID(&node);
        ImGui::SetCursorScreenPos(pos + padding);
        oldAnyActive = ImGui::IsAnyItemActive();
        ImGui::BeginGroup();
        graph.drawList->ChannelsSetCurrent(3); // foreground
        ImGui::Text(ID.c_str());
    }

    Node::~Node()
    {
        graph.drawList->ChannelsSetCurrent(2); // background
        ImGui::EndGroup();
        node.size = ImGui::GetItemRectSize() + padding + padding;
        bool nowAnyActive = !oldAnyActive && ImGui::IsAnyItemActive();
        ImVec2 farPos = pos + node.size;
        ImGui::SetCursorScreenPos(pos);
        ImGui::InvisibleButton("node", node.size);
        // if (ImGui::IsItemHovered()) {}
        if (ImGui::IsItemActive())
        {
            // if (nowAnyActive) {}
            if (ImGui::IsMouseDragging())
                node.pos = node.pos + ImGui::GetIO().MouseDelta;
        }
        graph.drawList->AddRectFilled(pos, farPos, ImColor(60, 60, 60), 0.0f);
        graph.drawList->AddRect(pos, farPos, ImColor(100, 100, 100), 0.0f);
        ImGui::PopID();
    }
    
    Socket::operator bool()
    {
        return rtn;
    }

    Socket::Socket(string ID, Node &_node)
    : node(_node), socket(_node.node.sockets[ID])
    {
        rtn = true;
        ImGui::BeginGroup();
        pos = ImGui::GetCursorScreenPos();
    }

    Socket::~Socket()
    {
        ImGui::EndGroup();
        float sockHeight = ImGui::GetItemRectSize().y / 2;
        pos.y += sockHeight;
        pos.x = socket.offset.x + node.pos.x + (socket.input ? 0 : node.node.size.x);
        node.graph.drawList->AddCircleFilled(pos, 5.0f, socket.color, 8);
        // do socket/link collision test
    }
}