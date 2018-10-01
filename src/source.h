// Copyright (c) 2018 Lucas Kleiss (LAK132)
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef SOURCE_H
#define SOURCE_H

#include <stdint.h>
#include <iostream>
using std::istream;
#include <fstream>
using std::ifstream;
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <string>
using std::string;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <cassert>

// ImGui
#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>
#include <misc/stl/imgui_stl.h>

#define APP_NAME "Network Maker"

#include <cql.h>
#include <lak/loopdata.h>
#include <lak/image.h>
#include <lak/texture.h>
#include <lak/pnm.h>
#include <lak/stream_util.h>
#include <lak/graphics.h>
#include <lak/json.h>
using lak::JSON::operator""_JSON;
using namespace lak::JSON;

#include "nodegraph.h"

struct userData_t
{
    float clearColor[4] = {0.0f, 0.3125f, 0.3125f, 1.0f};
    ImGuiIO *io = nullptr;
    ImGuiStyle *style = nullptr;

    lak::graph_t nodegraph;
};

#define LAK_MAIN_USERDATATYPE userData_t
#include <lak/main.h>

#endif // SOURCE_H