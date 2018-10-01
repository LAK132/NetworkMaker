/*
MIT License

Copyright (c) 2018 Lucas Kleiss (LAK132)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <vector>
#include <glm/vec2.hpp>

#include "ldebug.h"

#ifndef LAK_NO_SDL
#define LAK_USE_SDL
#endif

#ifndef LAK_NO_MULTITHREAD
#define LAK_USE_MULTITHREAD
#endif

#ifdef LAK_USE_SDL
#define SDL_MAIN_HANDLED
#include <SDL.h>
#endif // LAK_USE_SDL

#ifdef LAK_USE_MULTITHREAD
#include <thread>
#include <atomic>
#include <mutex>
#include "queue.h"
#endif // LAK_USE_MULTITHREAD

#ifndef LAK_LOOPDATA_H
#define LAK_LOOPDATA_H

// #ifndef LAK_USERDATATYPE
// #define LAK_USERDATATYPE void*
// #endif // LAK_USERDATATYPE

struct userData_t;

namespace lak
{
    using std::vector;
    #ifdef LAK_USE_MULTITHREAD
    using std::thread;
    using std::atomic;
    using std::atomic_bool;
    using std::mutex;
    using lak::queue_t;
    using lak::ticket_t;
    #endif // LAK_MAIN_MULTITHREAD

    struct screen_t
    {
        // pixel space
        size_t w;
        size_t h;
        // screen space
        float x;
        float y;
    };

    struct mouse_t
    {
        glm::vec2 pos = {0.0f, 0.0f};
        glm::vec2 delta = {0.0f, 0.0f};
        glm::ivec2 wheel = {0, 0};
        bool inUse = false;
        bool left = false;
        bool leftClicked = false;
        bool right = false;
        bool rightClicked = false;
        bool middle = false;
        bool middleClicked = false;
    };

    struct keyboard_t
    {
        bool inUse = false;

        char *text = nullptr;

        bool shift = false;
        bool alt = false;
        bool ctrl = false;

        bool capslock = false;
        bool numlock = false;

        bool tab = false;
        bool backspace = false;
        bool del = false;

        bool left = false;
        bool right = false;
        bool up = false;
        bool down = false;
    };

    struct loopData_t
    {
        #ifdef LAK_USE_MULTITHREAD
        queue_t windowq;
        queue_t drawq;
        atomic_bool updateNeedsContext = false;
        atomic_bool updateHasContext = false;
        atomic_bool running = true;
        #ifdef LAK_USE_SDL
        queue_t eventq;
        #endif // LAK_USE_SDL
        #else
        bool updateNeedsContext = false;
        const bool updateHasContext = true;
        bool running = true;
        #endif // LAK_USE_MULTITHREAD

        #ifdef LAK_USE_SDL
        SDL_Window* window;
        SDL_GLContext context;
        vector<SDL_Event> events;
        #endif // LAK_USE_SDL

        double targetDrawTime = 1.0/60.0;
        uint64_t drawTime = 0;
        double drawDelta = 0.0;
        uint64_t updateTime = 0;
        double updateDelta = 0.0;

        screen_t screen;
        mouse_t mouse;
        keyboard_t key;

        userData_t* userData;
    };
}

#endif // LAK_LOOPDATA_H