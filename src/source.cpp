#include "source.h"
#include <lak/main.cpp>

//https://gist.github.com/ocornut/7e9b3ec566a333d725d4

void draw(lak::loopData_t* loop_data)
{
    assert(loop_data != nullptr && loop_data->userData != nullptr);
    lak::loopData_t &ld = *loop_data;
    userData_t &ud = *ld.userData;

    glViewport(0, 0, ld.screen.w, ld.screen.h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // BEGIN RENDER CODE

    // END RENDER CODE

    if (ImDrawData *drawData = ImGui::GetDrawData())
        ImGui_ImplSdlGL3_RenderDrawData(drawData);
}

void update(lak::loopData_t* loop_data)
{
    assert(loop_data != nullptr && loop_data->userData != nullptr);
    lak::loopData_t &ld = *loop_data;
    userData_t &ud = *ld.userData;

    for (auto& event : ld.events)
    {
        switch(event.type)
        {
            case SDL_QUIT: {
                ld.running = false;
            } break;
        }
        ImGui_ImplSdlGL3_ProcessEvent(&event);
    }
    ld.events.clear();

    ImGui::NewFrame();

    if (auto graph = lak::Graph("Node Graph", ud.nodegraph))
    {
        for (auto node_ : graph.graph.nodes)
        if (auto node = lak::Node(node_.first, graph))
        {
            for (auto socket_ : node.node.sockets)
            if (auto socket = lak::Socket(socket_.first, node))
            {
                ImGui::Text("Hello, World!");
            }
        }
    }

    ImGui::Render();
}

void event(lak::loopData_t* loop_data)
{
    assert(loop_data != nullptr && loop_data->userData != nullptr);
    lak::loopData_t &ld = *loop_data;
    userData_t &ud = *ld.userData;

    SDL_Event event;
    while (SDL_PollEvent(&event) && ld.events.size() < 100)
        ld.events.push_back(event);

    ImGui_ImplSdlGL3_NewFrame(ld.window);
}

void init(lak::loopData_t* loop_data)
{
    assert(loop_data != nullptr);
    lak::loopData_t &ld = *loop_data;
    ld.userData = new userData_t;
    userData_t &ud = *ld.userData;

    ud.nodegraph.nodes["Node1"].sockets["Socket1"] = {true, ImColor(255, 255, 255, 255), ImVec2(0, 0)};
    ud.nodegraph.nodes["Node1"].sockets["Socket2"] = {false, ImColor(128, 255, 255, 255), ImVec2(0, 0)};
    ud.nodegraph.nodes["Node1"].sockets["Socket3"] = {true, ImColor(255, 128, 255, 255), ImVec2(0, 0)};
    ud.nodegraph.nodes["Node2"].sockets["Socket1"] = {false, ImColor(255, 255, 128, 255), ImVec2(0, 0)};
    ud.nodegraph.nodes["Node2"].sockets["Socket2"] = {true, ImColor(255, 255, 255, 255), ImVec2(0, 0)};
    ud.nodegraph.nodes["Node2"].sockets["Socket3"] = {false, ImColor(255, 128, 255, 255), ImVec2(0, 0)};
    ud.nodegraph.nodes["Node3"].sockets["Socket1"] = {true, ImColor(255, 255, 255, 255), ImVec2(0, 0)};
    ud.nodegraph.nodes["Node4"].sockets["Socket1"] = {false, ImColor(255, 255, 255, 255), ImVec2(0, 0)};
    
    assert(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) == 0);

    ld.screen.w = 1280;
    ld.screen.h = 720;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    ld.window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ld.screen.w, ld.screen.h, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    ld.context = SDL_GL_CreateContext(ld.window);
    SDL_GL_MakeCurrent(ld.window, ld.context);
    if (SDL_GL_SetSwapInterval(-1) == -1) // adaptive vsync
    {
        SDL_GL_SetSwapInterval(1); // standard vsync
    }

    gl3wInit();
    
    ImGui::CreateContext();
    ud.io = &ImGui::GetIO();
    ImGui_ImplSdlGL3_Init(ld.window);
    ImGui::StyleColorsDark();
    ud.style = &ImGui::GetStyle();
    ud.style->WindowRounding = 0;
    ImGui_ImplSdlGL3_NewFrame(ld.window);

    glViewport(0, 0, ld.screen.w, ld.screen.h);
    glClearColor(ud.clearColor[0], ud.clearColor[1], ud.clearColor[2], ud.clearColor[3]);
    glEnable(GL_DEPTH_TEST);

    // call the 3 main functions once from the main thread to ensure they all initialise in the right order
    event(loop_data);
    update(loop_data);
    draw(loop_data);
}

void destroy(lak::loopData_t* loop_data)
{
    assert(loop_data != nullptr && loop_data->userData != nullptr);
    lak::loopData_t &ld = *loop_data;
    userData_t &ud = *ld.userData;

    delete loop_data->userData;

    ImGui_ImplSdlGL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(loop_data->context);
    SDL_DestroyWindow(loop_data->window);

    SDL_Quit();
}