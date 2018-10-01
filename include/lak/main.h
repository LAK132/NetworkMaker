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

// #include <vector>

// #include "ldebug.h"

// #ifndef LAK_NO_SDL
// #define SDL_MAIN_HANDLED
// #include <SDL.h>
// #endif // LAK_NO_SDL

// #ifdef LAK_MAIN_MULTITHREAD
// #include <thread>
// #include <atomic>
// #include <mutex>
// #include "queue.h"
// #endif // LAK_MAIN_MULTITHREAD

#include <cmath>

#ifndef LAK_MAIN_H
#define LAK_MAIN_H

// #ifndef LAK_MAIN_USERDATATYPE
// #define LAK_MAIN_USERDATATYPE void*
// #endif // LAK_MAIN_USERDATATYPE

// namespace lak
// {
//     using std::vector;
//     #ifdef LAK_MAIN_MULTITHREAD
//     using std::thread;
//     using std::atomic;
//     using std::atomic_bool;
//     using std::mutex;
//     using lak::queue_t;
//     using lak::ticket_t;
//     #endif // LAK_MAIN_MULTITHREAD

//     struct loopData
//     {
//         #ifdef LAK_MAIN_MULTITHREAD
//         atomic_bool running;
//         queue_t windowq;
//         queue_t drawq;
//         atomic_bool updateNeedsContext = false;
//         atomic_bool updateHasContext = false;
//         #ifndef LAK_NO_SDL
//         queue_t eventq;
//         #endif // LAK_NO_SDL
//         #else
//         bool updateNeedsContext = false;
//         const bool updateHasContext = true;
//         bool running;
//         #endif // LAK_MAIN_MULTITHREAD

//         #ifndef LAK_NO_SDL
//         SDL_Window* window;
//         SDL_GLContext context;
//         vector<SDL_Event> events;
//         #endif // LAK_NO_SDL

//         double targetDrawTime = 1.0/60.0;
//         double drawDelta = 0.0;
//         double updateDelta = 0.0;
//         LAK_MAIN_USERDATATYPE userData;
//     };
// }

#include "loopdata.h"

void init(lak::loopData_t* loop_data);
void update(lak::loopData_t* loop_data);
void draw(lak::loopData_t* loop_data);
void destroy(lak::loopData_t* loop_data);
#ifdef LAK_USE_SDL
void event(lak::loopData_t* loop_data);
#endif // LAK_NO_SDL

// #define LAK_MAIN_IMPLEM
#ifdef LAK_MAIN_IMPLEM
#   ifndef LAK_MAIN_HAS_IMPLEM
#       define LAK_MAIN_HAS_IMPLEM
#       include "main.cpp"
#   endif // LAK_MAIN_HAS_IMPLEM
#endif // LAK_MAIN_IMPLEM

#endif // LAK_MAIN_H