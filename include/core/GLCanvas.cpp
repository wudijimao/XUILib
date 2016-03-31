//
//  GLCanvas.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "GLCanvas.hpp"
#include "XResManager.hpp"

//temp
#include <iostream>

RenderType GLCanvas::GetType()
{
    return RenderType::OPENGL;
}
void GLCanvas::pushRenderData(XDUILib::GLRenderData *data) {
    _needRenderDatas.push_back(data);
}

bool GLCanvas::InitGLProgram() {
    return _program.initWithFilePath(XResManager::pathForResource("shader/VertexShader", "vsh"), XResManager::pathForResource("shader/FragmentShader", "fsh"));
}

bool GLCanvas::InitFrameBuffer() {
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderBuffer);
    //声明并赋值一个GL枚举变量，赋值为检测GL_FRAMEBUFFER状态的返回值，
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //如果状态不完全
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }
    return true;
}

void GLCanvas::clear() {
    _needRenderDatas.clear();
}

bool GLCanvas::Present() {
    glViewport(0, 0, _size.Width(), _size.Height());
//    glClearColor(1, 1, 1, 1.0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto data : _needRenderDatas) {
        switch(data->Type()) {
            case XDUILib::GLRenderDataType::Square: {
                _program.enable();
                
                GLuint textureId = ((XDUILib::GLRenderSquareData*)data)->_textureId;
                if (textureId < 10) {
                    glActiveTexture(GL_ACTIVE_TEXTURE - 1);
                    glBindTexture(GL_TEXTURE_2D, textureId);
                    _program.setUniformValue("s_texture", GL_ACTIVE_TEXTURE - GL_TEXTURE0 - 1);
                    
                    _program.setUniformValue("useTexture", true);
                } else {
                    _program.setUniformValue("useTexture", false);
                }
                
                glUniformMatrix4fv(0, 1, true, (GLfloat*)&_transformMat);
                //glEnableVertexAttribArray(0);
                //glEnableVertexAttribArray(1);
                glBindVertexArray(data->_vectexArrayObject);
                
                // Draw
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                std::cout << "present HOme" << std::endl;
            }
                break;
            default:
                break;
        }
    }
    return true;
}


