//
//  GLCanvas.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "stdafx.hpp"
#include "IXCanvas.hpp"
#include "GL/GLRenderData.hpp"
#include "GL/GLHeaders.h"



class GLCanvas : public IXCanvas {
public:
    virtual RenderType GetType();
    void pushRenderData(XDUILib::GLRenderData *data);
protected:
    GLuint _renderBuffer;
    GLuint _framebuffer;
    GLuint _program;
    
    std::vector<XDUILib::GLRenderData*> _needRenderDatas;
protected:
    XResource::XSize _size;
    bool InitFrameBuffer();
private:
    bool checkShaderCompilState(GLuint shader);
    bool checkProgramCompilState(GLuint program);
public:
    bool InitGLProgram(const char *vertexShaderText, const char *fragmentShaderText);
    virtual bool Present();
};
