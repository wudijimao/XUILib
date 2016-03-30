//
//  GLProgram.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/27.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#include <iostream>
#include "GLProgram.hpp"


bool GLProgram::checkShaderCompilState(GLuint shader) {
    GLint compileResult;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (GL_FALSE == compileResult)
    {
        GLint logLen;
        //得到编译日志长度
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0)
        {
            char *log = new char[logLen];
            GLsizei written;
            //得到日志信息并输出
            glGetShaderInfoLog(shader, logLen,&written, log);
            std::cout << log << std::endl;
            delete[] log;//释放空间
        }
        return false;
    }
    return true;
}
bool GLProgram::checkProgramCompilState(GLuint program) {
    GLint compileResult;
    glGetProgramiv(program, GL_LINK_STATUS, &compileResult);
    if (GL_FALSE == compileResult) {
        GLint logLen;
        //得到编译日志长度
        glGetProgramiv(program,GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0)
        {
            char *log = new char[logLen];
            GLsizei written;
            //得到日志信息并输出
            glGetProgramInfoLog(program, logLen, &written, log);
            std::cout << log << std::endl;
            delete[] log;//释放空间
        }
        return false;
    }
    return true;
}

bool GLProgram::init(const char *vertexShaderText, const char *fragmentShaderText) {
    if (vertexShaderText == nullptr || fragmentShaderText == nullptr) {
        return false;
    }
    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderText, NULL);
    glCompileShader(vertexShader);
    if (!checkShaderCompilState(vertexShader))
    {
        return false;
    }
    
    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderText, NULL);
    glCompileShader(fragmentShader);
    if (!checkShaderCompilState(fragmentShader))
    {
        return false;
    }
    
    // Create and link program
    _program = glCreateProgram();
    glAttachShader(_program, vertexShader);
    glAttachShader(_program, fragmentShader);
    glLinkProgram(_program);
    if (!checkProgramCompilState(_program))
    {
        return false;
    }
    // Use program
    glUseProgram(_program);
    return true;
}

bool GLProgram::initWithFilePath(const char *vFilePath, const char *fFilePath) {
    auto vsh = XResource::XString::stringWithContentOfFile(vFilePath);
    auto fsh = XResource::XString::stringWithContentOfFile(fFilePath);
    return init(vsh->getUTF8String()->c_str(), fsh->getUTF8String()->c_str());
    /*return init("#version 300 es\
                \
                precision mediump float;\
                \
                in vec3 inPos;\
                in vec2 vTexCoord;\
                in vec4 inColor;\
                \
                out vec4 Color;\
                out vec2 texCoord;\
                \
                uniform mat4 myMat;\
                \
                void main() {\
                gl_Position = vec4(inPos, 1.0) * myMat;\
                texCoord = vTexCoord;\
                Color = inColor;\
                }", "#version 300 es\
                precision mediump float;\
                \
                in vec4 Color;\
                in vec2 texCoord;\
                out vec4 fragmentColor;\
                \
                uniform bool useTexture;\
                uniform sampler2D s_texture;\
                \
                void main() {\
                    //if(useTexture) {\
                    //    fragmentColor = texture(s_texture, texCoord) * Color;\
                    //} else {\
                     //   fragmentColor = Color;\
                    //}\
                }");*/
}

bool GLProgram::setUniformValue(const char *uniformName, const XResource::XColor &color) {
    GLuint location = glGetAttribLocation(_program, uniformName);
    glUniform4f(location, color.r, color.b, color.b, color.a);
    return true;
}

bool GLProgram::setUniformValue(const char *uniformName, bool b) {
    GLuint location = glGetAttribLocation(_program, uniformName);
    int temp = (int)b;
    glUniform1i(location, temp);
    return true;
}
bool GLProgram::enable() {
    glUseProgram(_program);
    GLint error = glGetError();
    return true;
}




