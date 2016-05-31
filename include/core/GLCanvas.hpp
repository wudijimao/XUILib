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


struct SIMPLEDIRECTUI_API GLTransform3D {
public:
    GLTransform3D() {
        _transformMat[0] = 1;
        _transformMat[5] = 1;
        _transformMat[10] = 1;
        _transformMat[15] = 1;
    }
    GLfloat _transformMat[16];
    void setScale(GLfloat x, GLfloat y) {
        _transformMat[0] = x;
        _transformMat[5] = y;
    }
    void setScale(GLfloat x, GLfloat y, GLfloat z) {
        _transformMat[0] = x;
        _transformMat[5] = y;
        _transformMat[10] = z;
    }
    void setPosition(GLfloat x, GLfloat y) {
        _transformMat[12] = x;
        _transformMat[13] = y;
    }
    void setPosition(GLfloat x, GLfloat y, GLfloat z) {
        _transformMat[12] = x;
        _transformMat[13] = y;
        _transformMat[14] = z;
    }
};


class SIMPLEDIRECTUI_API GLCanvas : public IXCanvas {
public:
    GLCanvas() {
        _transform.setPosition(-1, 1);
    }
    virtual RenderType GetType();
    void pushRenderData(XDUILib::GLRenderData **data, int size);
	XResource::XSize GetSize() {
		return _size;
	}
protected:
    GLuint _renderBuffer;
    GLuint _framebuffer;
    
    std::vector<XDUILib::GLRenderData*> _needRenderDatas;
protected:
    XResource::XSize _size;
    XResource::XSize _pixelSize;
    double mHighResolutionPixelScale;
    bool InitFrameBuffer();
    bool InitGLProgram();
    void enableGLSettings();
    virtual void setSize(const XResource::XSize &size, double highResolutionPixelScale) {
        _size = size;
        mHighResolutionPixelScale = highResolutionPixelScale;
        _pixelSize.Width(_size.Width() * highResolutionPixelScale);
        _pixelSize.Height(_size.Height() * highResolutionPixelScale);
        _transform.setScale(2.0 / (float)size.Width(), -2.0 / (float)size.Height());
        
    }
public:
    virtual bool Present();
    virtual void clear();
private:
    GLTransform3D _transform;
    //temp
    GLProgram _program;
};
