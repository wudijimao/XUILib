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
#include "GL/GLProgram.hpp"


class SIMPLEDIRECTUI_API GLCanvas : public IXCanvas {
public:
    GLCanvas() {
        _transformMat[10] = 1;
        _transformMat[12] = -1;
        _transformMat[13] = 1;
        _transformMat[15] = 1;
    }
    virtual RenderType GetType();
    void pushRenderData(XDUILib::GLRenderData *data);
	XResource::XSize GetSize() {
		return _size;
	}
protected:
    GLuint _renderBuffer;
    GLuint _framebuffer;
    
    std::vector<XDUILib::GLRenderData*> _needRenderDatas;
protected:
    XResource::XSize _size;
    bool InitFrameBuffer();
    bool InitGLProgram();
    virtual void setSize(const XResource::XSize &size) {
        _size = size;
        _transformMat[0] = 2.0 / (float)size.Width();
        _transformMat[5] = -2.0 / (float)size.Height();
    }
public:
    virtual bool Present();
    virtual void clear();
private:
    GLfloat _transformMat[16];
    //temp
    GLProgram _program;
};
