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

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <iostream>
#include <assert.h>
#include <GL/gl3w.h>

#include "ldebug.h"
#include "texture.h"
#include "stride_vector.h"
#include "stream_util.h"

#ifndef LAK_GRAPHICS_H
#define LAK_GRAPHICS_H

namespace lak
{
    using std::shared_ptr;
    using std::make_shared;
    using std::weak_ptr;
    using std::vector;
    using std::string;
    using std::pair;
    using std::make_pair;
    using std::unordered_map;
    using std::istream;
    using std::endl;
    struct shader_t;
    struct mesh_t;

    bool initShader(const string& code, GLuint program, GLenum type);
    bool initShader(string&& code, GLuint program, GLenum type);
    bool linkProgram(GLuint program);

    struct renderParams_t
    {
        GLenum frontFaceMode    = GL_FILL;  // glPolygonMode(GL_FRONT, frontFaceMode);
        GLenum backFaceMode     = GL_LINE;  // glPolygonMode(GL_BACK, backFaceMode);

        bool blend              = false;    // glEnable/glDisable(GL_BLEND);
        GLclampf blendColor[4]  = {0,0,0,0};// glBlendColor(...);
        // glBlendFuncSeparate(blendSrcRBG, blendDstRGB, blendSrcAlpha, blendDstAlpha);
        GLenum blendSrcRBG      = GL_ONE;
        GLenum blendDstRGB      = GL_ZERO;
        GLenum blendDstAlpha    = GL_ONE;
        GLenum blendSrcAlpha    = GL_ZERO;
        // glBlendEquationSeparate(blendEqnRGB, blendEqnAlpha);
        GLenum blendEqnRGB      = GL_FUNC_ADD;
        GLenum blendEqnAlpha    = GL_FUNC_ADD;

        bool dither             = false;    // glEnable/glDisable(GL_DITHER);

        bool cullFace           = false;    // glEnable/glDisable(GL_CULL_FACE);
        GLenum cullFaceMode     = GL_BACK;  // glCullFace(cullFaceMode);

        bool depthTest          = false;    // glEnable/glDisable(GL_DEPTH_TEST);
        GLenum depthFunc        = GL_LESS;  // glDepthFunc(depthFunc);
        GLclampd depthTestNear  = 0;        // glDepthRange(depthTestNear, depthTestFar);
        GLclampd depthTestFar   = 1;

        bool scissorTest        = false;    // glEnable/glDisable(GL_SCISSOR_TEST);
    };

    void renderParams(renderParams_t *params, renderParams_t *previous = nullptr);

    struct element_t;

    struct vertexArray_t
    {
        struct buffer_t
        {
        private:
            bool isInit = false;
            stride_vector back;
        public:
            unordered_map<string, element_t> elements;
            GLuint vertBuff = 0;
            size_t size = 0;
            GLenum usage = GL_STATIC_DRAW;
            void bind();
            bool update();
            ~buffer_t();
        };
    private:
        bool isInit = false;
    public:
        vector<buffer_t> buffer;
        GLuint vertArr = 0, indxBuff = 0;
        void init();
        void bind();
        ~vertexArray_t();
    };

    struct element_t
    {
    protected:
        stride_vector data;
        friend vertexArray_t::buffer_t;
    public:
        size_t size = 0;
        size_t stride = 0;
        size_t interlStride = 0;
        GLintptr offset = 0;    // buffer position (read only)
        GLuint divisor = 0;     // buffer step per vertex (0=1/vtx, 1=1/mesh, 2=1/2mesh, etc...)
        bool normalized = false;
        bool active = false;
        bool dirty = false;
        element_t();
        template <typename T>
        element_t(const vector<T> &datavec)
        {
            data = datavec;
            stride = data.stride;
            size = data.size();
            dirty = true;
        }
        template <typename T>
        element_t(vector<T> &&datavec)
        {
            data = datavec;
            stride = data.stride;
            size = data.size();
            dirty = true;
        }
        template <typename T>
        inline const T& get(size_t idx) const
        {
            return data.at<T>(idx);
        }
        // calling this will dirty the buffer
        template <typename T>
        inline T& set(size_t idx)
        {
            dirt = true;
            if (sizeof(T) * idx > data.size())
            {
                size_t newsize = sizeof(T) * idx;
                newsize += stride - (newsize % stride);
                data.init(newsize, stride);
            }
            return data.at<T>(idx);
        }
        template <typename T>
        void set(size_t idx, size_t length, const T *rhs)
        {
            dirty = true;
            memcpy(&data.at<T>(idx), rhs, length * sizeof(T));
        }
        template <typename T>
        inline void set(size_t idx, const vector<T> &rhs)
        {
            dirty = true;
            if (sizeof(T) * (idx + rhs.size()) > data.size())
            {
                size_t newsize = sizeof(T) * (idx + rhs.size());
                newsize += stride - (newsize % stride);
                data.init(newsize, stride);
            }
            memcpy(&data.at<T>(idx), &rhs[0], rhs.size() * sizeof(T));
        }
        template <typename T>
        inline void set(size_t idx, vector<T> &&rhs)
        {
            dirty = true;
            if (sizeof(T) * (idx + rhs.size()) > data.size())
            {
                size_t newsize = sizeof(T) * (idx + rhs.size());
                newsize += stride - (newsize % stride);
                data.init(newsize, stride);
            }
            memcpy(&data.at<T>(idx), &rhs[0], rhs.size() * sizeof(T));
        }
        template <typename T>
        void operator=(const vector<T> &datavec)
        {
            data = datavec;
            stride = data.stride;
            size = data.size();
            dirty = true;
        }
        template <typename T>
        void operator=(vector<T> &&datavec)
        {
            data = datavec;
            stride = data.stride;
            size = data.size();
            dirty = true;
        }
        template <typename T>
        T* init(size_t dlength)
        {
            stride = sizeof(T);
            size = stride * dlength;
            data.init(stride, size);
            dirty = true;
            return data.get<T>();
        }
    };

    struct mesh_t
    {
    private:
        bool index_mode = false;
    public:
        vertexArray_t vertArray;
        shared_ptr<shader_t> shader;
        unordered_map<string, shared_ptr<texture_t>> textures;
        vector<GLuint> index;

        // config
        GLenum drawMode = GL_TRIANGLES;

        size_t indexCount; // must be set by user if index array not used

        bool dirty = false; // should draw call update first?
        void update();
        void draw(GLsizei count = 1, const void *indexOffset = NULL);
    };

    struct attribute_t
    {
        GLuint position = -1;   // program position
        GLenum type = GL_FLOAT; // element type
        GLint size = 4;         // element count
        string name = "";       // name in shader
        bool active = false;    // is the element currenly in use
    };

    struct shader_t
    {
        shared_ptr<GLint> program;
        unordered_map<string, attribute_t> attributes;
        unordered_map<string, attribute_t> uniforms;
        shader_t();
        ~shader_t();
        shader_t(const shader_t& other);
        shader_t(shader_t&& other);
        shader_t(const string& vshader, const string& fshader);
        shader_t& operator=(const shader_t& other);
        shader_t& operator=(shader_t&& other);
        bool init(const string& vshader, const string& fshader);
        void initAttribs();
        void setUniform(const string& name, const void* data, GLsizei size = 1, GLboolean transpose = GL_FALSE);
        void enable();
        GLint operator*() const;
    };
    
    shader_t operator"" _shader (const char *str, const size_t size);
}

// #define LAK_GRAPHICS_IMPLEM
#ifdef LAK_GRAPHICS_IMPLEM
#   ifndef LAK_GRAPHICS_HAS_IMPLEM
#       define LAK_GRAPHICS_HAS_IMPLEM
#       include "graphics.cpp"
#   endif // LAK_GRAPHICS_HAS_IMPLEM
#endif // LAK_GRAPHICS_IMPLEM

#endif // LAK_GRAPHICS_H