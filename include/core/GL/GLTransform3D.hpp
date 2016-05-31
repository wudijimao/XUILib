//
//  GLTransform3D.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/31.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include "GLHeaders.h"

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