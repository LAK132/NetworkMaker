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

#include <filesystem>

#ifndef LAK_FILE_H
#define LAK_FILE_H

namespace  lak
{
    namespace fs = std::filesystem;
    template<typename FILE>
    struct file_t
    {
        FILE file;
        fs::path path;
        bool fileOpen;
        bool pathValid;
        file_t(bool open = false) :                                         fileOpen(open), pathValid(false) {}
        file_t(const fs::path &p, bool valid = true, bool open = false) :   fileOpen(open), path(p), pathValid(valid) {}
        file_t(fs::path &&p, bool valid = true, bool open = false) :        fileOpen(open), path(p), pathValid(valid) {}
        file_t(const FILE &f, bool open = true) :                           file(f), fileOpen(open), pathValid(false) {}
        file_t(FILE &&f, bool open = true) :                                file(f), fileOpen(open), pathValid(false) {}
        file_t(const FILE &f, const fs::path &p, bool valid = true) :       file(f), fileOpen(true), path(p), pathValid(valid) {}
        file_t(const FILE &f, fs::path &&p, bool valid = true) :            file(f), fileOpen(true), path(p), pathValid(valid) {}
        file_t(FILE &&f, const fs::path &p, bool valid = true) :            file(f), fileOpen(true), path(p), pathValid(valid) {}
        file_t(FILE &&f, fs::path &&p, bool valid = true) :                 file(f), fileOpen(true), path(p), pathValid(valid) {}
    };
}
#endif