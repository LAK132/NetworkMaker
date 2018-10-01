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

#include "graphics.h"

namespace lak
{
    bool initShader(const string& code, GLuint program, GLenum type)
    {
        GLuint shader = glCreateShader(type);
        GLchar* shadercstr = (GLchar*)&(code[0]);
        glShaderSource(shader, 1, &shadercstr, NULL);
        glCompileShader(shader);
        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if(!compiled)
        {
            GLint msgSize = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msgSize);
            string msg; msg.resize(msgSize);
            glGetShaderInfoLog(shader, msgSize, NULL, &(msg[0]));
            LERROR("Error compiling shader " << code << endl << msg << endl);
            return false;
        }
        glAttachShader(program, shader);
        return true;
    }
    bool initShader(string&& code, GLuint program, GLenum type)
    {
        return initShader(code, program, type);
    }
    bool linkProgram(GLuint program)
    {
        glLinkProgram(program);
        GLint linked;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if(!linked)
        {
            GLint msgSize;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &msgSize);
            string msg; msg.resize(msgSize);
            glGetProgramInfoLog(program, msgSize, NULL, &(msg[0]));
            LERROR("Shader program failed to link" << endl << msg << endl);
            return false;
        }
        return true;
    }

    void renderParams(renderParams_t *params, renderParams_t *previous)
    {
        static renderParams_t curr;
        if (previous != nullptr) *previous = curr;
        if (params == nullptr) return;

        if (params->frontFaceMode != curr.frontFaceMode)
            glPolygonMode(GL_FRONT, params->frontFaceMode);
        if (params->backFaceMode != curr.backFaceMode)
            glPolygonMode(GL_BACK, params->backFaceMode);
        
        if (params->blend != curr.blend)
            params->blend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
        if (params->blendColor != curr.blendColor)
            glBlendColor(params->blendColor[0], params->blendColor[1], params->blendColor[2], params->blendColor[3]);
        if (params->blendSrcRBG != curr.blendSrcRBG || params->blendSrcAlpha != curr.blendSrcAlpha ||
            params->blendDstRGB != curr.blendDstRGB || params->blendDstAlpha != curr.blendDstAlpha)
            glBlendFuncSeparate(params->blendSrcRBG, params->blendDstRGB, params->blendDstRGB, params->blendSrcAlpha);
        if (params->blendEqnRGB != curr.blendEqnRGB || params->blendEqnAlpha != curr.blendEqnAlpha)
            glBlendEquationSeparate(params->blendEqnRGB, params->blendEqnAlpha);

        if (params->dither != curr.dither)
            params->dither ? glEnable(GL_DITHER) : glDisable(GL_DITHER);

        if (params->cullFace != curr.cullFace)
            params->cullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
        if (params->cullFaceMode != curr.cullFaceMode)
            glCullFace(params->cullFaceMode);
        
        if (params->depthTest != curr.depthTest)
            params->depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        if (params->depthFunc != curr.depthFunc)
            glDepthFunc(params->depthFunc);
        if (params->depthTestNear != curr.depthTestNear || params->depthTestFar != params->depthTestFar)
            glDepthRange(params->depthTestNear, params->depthTestFar);

        if (params->scissorTest != curr.scissorTest)
            params->scissorTest ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);

        curr = *params;
    }

    // shader_t
    shader_t::shader_t(){}

    shader_t::~shader_t()
    {
        if(program.unique()) glDeleteProgram(*program);
    }

    shader_t::shader_t(const shader_t& other)
    { 
        *this = other; 
    }

    shader_t::shader_t(shader_t&& other)
    { 
        *this = other; 
    }
    
    shader_t::shader_t(const string& vshader, const string& fshader)
    {
        program = make_shared<GLint>(glCreateProgram());
        assert(initShader(vshader, *program, GL_VERTEX_SHADER));
        assert(initShader(fshader, *program, GL_FRAGMENT_SHADER));
        assert(linkProgram(*program));
        initAttribs();
    }
    
    shader_t& shader_t::operator=(const shader_t& other)
    { 
        program = other.program;
        attributes = other.attributes;
        uniforms = other.uniforms;
        return *this; 
    }

    shader_t& shader_t::operator=(shader_t&& other)
    { 
        return *this = other; 
    }

    bool shader_t::init(const string& vshader, const string& fshader)
    {
        program = make_shared<GLint>(glCreateProgram());
        if(!initShader(vshader, *program, GL_VERTEX_SHADER)) return false;
        if(!initShader(fshader, *program, GL_FRAGMENT_SHADER)) return false;
        if(!linkProgram(*program)) return false;
        initAttribs();
        return true;
    }

    void shader_t::initAttribs()
    {
        glUseProgram(*program);

        GLint count;
        vector<GLchar> name;
        GLsizei length;
        GLint size;
        GLenum type;

        GLint nameLen = 0;
        glGetProgramiv(*program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &nameLen);
        name.resize(nameLen);

        attributes.clear();
        glGetProgramiv(*program, GL_ACTIVE_ATTRIBUTES, &count);
        for (GLint i = 0; i < count; i++)
        {
            // FFR: type = advanced type ie GL_UNSIGNED_INT_VEC4, size = units of type (?)
            // attr.type = basic type ie GL_UNSIGNED_INT, attr.size = units of attr.type
            // where type = GL_FLOAT_VEC4 and size = 1 then attr.type = GL_FLOAT and attr.size = 4 (??)
            // https://www.gamedev.net/forums/topic/545284-glsl-glgetactiveattrib---the-size-parameter/
            for(size_t i = 0; i < name.size(); i++) name[i] = '\0';
            glGetActiveAttrib(*program, (GLuint)i, nameLen, &length, &size, &type, &name[0]);
            attribute_t attr;
            attr.position = glGetAttribLocation(*program, &name[0]);
            // glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, (GLint*)&attr.type);
            // glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, &attr.size);
            attr.name = &name[0];
            attributes[attr.name] = attr;
        }

        glGetProgramiv(*program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameLen);
        name.resize(nameLen);
        
        uniforms.clear();
        glGetProgramiv(*program, GL_ACTIVE_UNIFORMS, &count);
        for (GLint i = 0; i < count; i++)
        {
            // FFR: type == attr.type
            // size == attr.size (number of type in array, IF it is an array, else 1)
            for(size_t i = 0; i < name.size(); i++) name[i] = '\0';
            attribute_t attr;
            glGetActiveUniform(*program, (GLuint)i, nameLen, &length, &attr.size, &attr.type, &name[0]);
            attr.position = glGetUniformLocation(*program, &name[0]);
            attr.name = &name[0];
            uniforms[attr.name] = attr;
        }
    }

    void shader_t::setUniform(const string& name, const void* data, GLsizei size, GLboolean transpose)
    {
        if (program.use_count() <= 0) return;
        auto it = uniforms.find(name);
        if (it == uniforms.end()) { 
            LDEBUG("Failed to find uniform \"" << name << "\""); 
            LDEBUG("Available uniforms are :");
            for (auto it : uniforms) LDEBUG(it.first);
            return; 
        }
        glUseProgram(*program);
        attribute_t& attr = it->second;
        if(attr.position < 0) return;
        switch (attr.type)
        {
            case GL_FLOAT: 
                glUniform1fv(attr.position, size, (GLfloat*)data); break;
            case GL_FLOAT_VEC2:
                glUniform2fv(attr.position, size, (GLfloat*)data); break;
            case GL_FLOAT_VEC3:
                glUniform3fv(attr.position, size, (GLfloat*)data); break;
            case GL_FLOAT_VEC4:
                glUniform4fv(attr.position, size, (GLfloat*)data); break;
            case GL_FLOAT_MAT2:
                glUniformMatrix2fv(attr.position, size, transpose, (GLfloat*)data); break;
            case GL_FLOAT_MAT2x3:
                glUniformMatrix2x3fv(attr.position, size, transpose, (GLfloat*)data); break;
            case GL_FLOAT_MAT2x4:
                glUniformMatrix2x4fv(attr.position, size, transpose, (GLfloat*)data); break;
            case GL_FLOAT_MAT3:
                glUniformMatrix3fv(attr.position, size, transpose, (GLfloat*)data); break;
            case GL_FLOAT_MAT3x2:
                glUniformMatrix3x2fv(attr.position, size, transpose, (GLfloat*)data); break;
            case GL_FLOAT_MAT3x4:
                glUniformMatrix3x4fv(attr.position, size, transpose, (GLfloat*)data); break;
            case GL_FLOAT_MAT4:
                glUniformMatrix4fv(attr.position, size, transpose, (GLfloat*)data); break;
            case GL_FLOAT_MAT4x2:
                glUniformMatrix4x2fv(attr.position, size, transpose, (GLfloat*)data); break;
            case GL_FLOAT_MAT4x3:
                glUniformMatrix4x3fv(attr.position, size, transpose, (GLfloat*)data); break;
            case GL_DOUBLE: 
                glUniform1dv(attr.position, size, (GLdouble*)data); break;
            case GL_DOUBLE_VEC2:
                glUniform2dv(attr.position, size, (GLdouble*)data); break;
            case GL_DOUBLE_VEC3:
                glUniform3dv(attr.position, size, (GLdouble*)data); break;
            case GL_DOUBLE_VEC4:
                glUniform4dv(attr.position, size, (GLdouble*)data); break;
            case GL_DOUBLE_MAT2:
                glUniformMatrix2dv(attr.position, size, transpose, (GLdouble*)data); break;
            case GL_DOUBLE_MAT2x3:
                glUniformMatrix2x3dv(attr.position, size, transpose, (GLdouble*)data); break;
            case GL_DOUBLE_MAT2x4:
                glUniformMatrix2x4dv(attr.position, size, transpose, (GLdouble*)data); break;
            case GL_DOUBLE_MAT3:
                glUniformMatrix3dv(attr.position, size, transpose, (GLdouble*)data); break;
            case GL_DOUBLE_MAT3x2:
                glUniformMatrix3x2dv(attr.position, size, transpose, (GLdouble*)data); break;
            case GL_DOUBLE_MAT3x4:
                glUniformMatrix3x4dv(attr.position, size, transpose, (GLdouble*)data); break;
            case GL_DOUBLE_MAT4:
                glUniformMatrix4dv(attr.position, size, transpose, (GLdouble*)data); break;
            case GL_DOUBLE_MAT4x2:
                glUniformMatrix4x2dv(attr.position, size, transpose, (GLdouble*)data); break;
            case GL_DOUBLE_MAT4x3:
                glUniformMatrix4x3dv(attr.position, size, transpose, (GLdouble*)data); break;
            case GL_INT:
                glUniform1iv(attr.position, size, (GLint*)data); break;
            case GL_INT_VEC2:
                glUniform2iv(attr.position, size, (GLint*)data); break;
            case GL_INT_VEC3:
                glUniform3iv(attr.position, size, (GLint*)data); break;
            case GL_INT_VEC4:
                glUniform4iv(attr.position, size, (GLint*)data); break;
            case GL_UNSIGNED_INT:
                glUniform1uiv(attr.position, size, (GLuint*)data); break;
            case GL_UNSIGNED_INT_VEC2:
                glUniform2uiv(attr.position, size, (GLuint*)data); break;
            case GL_UNSIGNED_INT_VEC3:
                glUniform3uiv(attr.position, size, (GLuint*)data); break;
            case GL_UNSIGNED_INT_VEC4:
                glUniform4uiv(attr.position, size, (GLuint*)data); break;
            default: break;
        }
    }

    void shader_t::enable()
    {
        if (program.use_count() <= 0) { LERRLOG("Shader program use count = 0"); return; }
        GLint currProgram; glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
        if (currProgram != *program)
            glUseProgram(*program);
    }

    GLint shader_t::operator*() const
    {
        return *program;
    }

    // vertexArray_t::buffer_t
    vertexArray_t::buffer_t::~buffer_t()
    {
        glDeleteBuffers(1, &vertBuff);
    }

    void vertexArray_t::buffer_t::bind()
    {
        if (!isInit)
        {
            glGenBuffers(1, &vertBuff);
            isInit = true;
        }
        GLint current;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &current);
        if (current != vertBuff) glBindBuffer(GL_ARRAY_BUFFER, vertBuff);
    }

    struct buffer_group
    {
        vector<stride_vector*> elems = {};
        size_t stride = 0;
    };

    bool vertexArray_t::buffer_t::update()
    {
        bind();
        bool dirty = false;

        for (auto &it : elements)
            if (it.second.dirty) { dirty = true; break; }

        if (dirty)
        {
            buffer_group group;
            group.elems.reserve(elements.size());
            for (auto &it : elements)
            {
                if (!it.second.active) continue;
                it.second.dirty = false;
                it.second.offset = group.stride; // calculate the buffer offset on the fly
                group.elems.push_back(&(it.second.data));
                group.stride += it.second.stride;
            }

            for (auto &it : elements)
                it.second.interlStride = group.stride;

            stride_vector front = stride_vector::interleave(group.elems);

            if (front.size() == size)
            {
                size_t offset = 0;
                while (offset < size && front[offset] == back[offset]) ++offset;
                if (offset < size)
                {
                    size_t length = size;
                    while (length > offset && front[length] == back[length]) --length;
                    length = length - offset;
                    glBufferSubData(GL_ARRAY_BUFFER, offset, length, &front.data[offset]);
                    back = front;
                }
            }
            else
            {
                size = front.size();
                glBufferData(GL_ARRAY_BUFFER, size, &front.data[0], usage);
                back = front;
            }
        }

        return dirty;
    }

    // vertexArray_t
    void vertexArray_t::init()
    {
        if(isInit)
        {
            glDeleteVertexArrays(1, &vertArr);
            glDeleteBuffers(1, &indxBuff);
        }
        glGenVertexArrays(1, &vertArr);
        glGenBuffers(1, &indxBuff);
        isInit = true;
    }

    void vertexArray_t::bind()
    {
        if(!isInit) init();
        GLint current; 

        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current);
        if (current != vertArr) glBindVertexArray(vertArr);
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &current);
        if (current != indxBuff) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indxBuff);
    }

    vertexArray_t::~vertexArray_t()
    {
        if(!isInit) return;
        glDeleteVertexArrays(1, &vertArr);
        glDeleteBuffers(1, &indxBuff);
        isInit = false;
    }

    // element_t
    element_t::element_t(){}

    // mesh_t
    void mesh_t::update()
    {
        vertArray.bind();
        size_t bufferCount = vertArray.buffer.size();
        index_mode = index.size() > 0;

        if (shader.use_count() > 0)
        {
            for (size_t i = 0; i < bufferCount; ++i)
            {
                auto &buff = vertArray.buffer[i];
                if (!buff.update())
                    continue;
                for (auto &elem : buff.elements)
                {
                    auto &attr = shader->attributes.find(elem.first);
                    if (attr != shader->attributes.end())
                    {
                        if (elem.second.active)
                        {
                            glEnableVertexAttribArray(attr->second.position);
                            glVertexAttribDivisor(attr->second.position, elem.second.divisor);
                            glVertexAttribPointer(attr->second.position, attr->second.size, attr->second.type, elem.second.normalized, elem.second.interlStride, (GLvoid*)elem.second.offset);
                            attr->second.active = true;
                        }
                        else
                        {
                            glDisableVertexAttribArray(attr->second.position);
                            attr->second.active = false;
                        }
                    }
                }
            }
        }
        else for (auto &buff : vertArray.buffer) buff.update();

        if(index_mode)
        {
            indexCount = index.size();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), &(index[0]), GL_STATIC_DRAW);
        }

        dirty = false;
    }

    void mesh_t::draw(GLsizei count, const void *indexOffset)
    {
        if (dirty)
            update();
        vertArray.bind();
        if (shader.use_count() > 0)
            shader->enable();
        for (auto &tex : textures)
            if (tex.second.use_count() > 0)
                tex.second->bind();
        if (index_mode)
            glDrawElementsInstanced(drawMode, indexCount, GL_UNSIGNED_INT, indexOffset, count);
        else
            glDrawArraysInstanced(drawMode, 0, indexCount, count);
    }

    shader_t operator"" _shader (const char *str, const size_t size)
    {
        string _str = str;
        size_t vshaderpos = _str.find("#VERTEX_SHADER");
        size_t fshaderpos = _str.find("#FRAGMENT_SHADER");

        string vshader, fshader;
        if (vshaderpos < fshaderpos)
        {
            vshaderpos = _str.find_first_of("\n", vshaderpos);
            vshader = _str.substr(vshaderpos, fshaderpos - vshaderpos);
            fshaderpos = _str.find_first_of("\n", fshaderpos);
            fshader = _str.substr(fshaderpos);
        }
        else
        {
            fshaderpos = _str.find_first_of("\n", fshaderpos);
            fshader = _str.substr(fshaderpos, vshaderpos - fshaderpos);
            vshaderpos = _str.find_first_of("\n", vshaderpos);
            vshader = _str.substr(vshaderpos);
        }

        return shader_t(vshader, fshader);
    }
}