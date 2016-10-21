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
    GLTransform3D();
    GLTransform3D(const GLTransform3D &left, const GLTransform3D &right);
    GLfloat _transformMat[16];
    void setScale(GLfloat x, GLfloat y);
    void setScale(GLfloat x, GLfloat y, GLfloat z);
    void setPosition(GLfloat x, GLfloat y);
    void move(GLfloat x, GLfloat y);
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    GLTransform3D operator * (const GLTransform3D&right);
};