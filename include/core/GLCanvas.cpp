//
//  GLCanvas.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "GLCanvas.hpp"


GLfloat transformMat[] = {
    1.0f/150.0f, 0, 0, 0,
    0, 1.0f/150.0f, 0, 0,
    0, 0, 1, 0,
    -1, -1, 0, 1,};


RenderType GLCanvas::GetType()
{
    return RenderType::OPENGL;
}
void GLCanvas::pushRenderData(XDUILib::GLRenderData *data) {
    _needRenderDatas.push_back(data);
}

bool GLCanvas::InitFrameBuffer() {
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderBuffer);
    glViewport(0, 0, _size.Width(), _size.Height());
    glClearColor(0, 1, 0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //声明并赋值一个GL枚举变量，赋值为检测GL_FRAMEBUFFER状态的返回值，
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //如果状态不完全
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }
    return true;
}

bool GLCanvas::checkShaderCompilState(GLuint shader) {
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
            delete[] log;//释放空间
        }
        return false;
    }
    return true;
}
bool GLCanvas::checkProgramCompilState(GLuint program) {
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
            delete[] log;//释放空间
        }
        return false;
    }
    return true;
}

bool GLCanvas::InitGLProgram(const char *vertexShaderText, const char *fragmentShaderText) {
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

bool GLCanvas::Present() {
    for (auto data : _needRenderDatas) {
        switch(data->Type()) {
            case XDUILib::GLRenderDataType::Square: {
                glUniformMatrix4fv(0, 1, true, (GLfloat*)&transformMat);
                //glEnableVertexAttribArray(0);
                //glEnableVertexAttribArray(1);
                glBindVertexArray(data->_vectexArrayObject);
                // Draw
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
                break;
            default:
                break;
        }
    }
    return true;
}

