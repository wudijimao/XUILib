//
//  GLTransform3D.cpp
//  XDUILib
//
//  Created by ximiao on 16/5/31.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "GLTransform3D.hpp"


GLTransform3D::GLTransform3D() {
    _transformMat[0] = 1;
    _transformMat[5] = 1;
    _transformMat[10] = 1;
    _transformMat[15] = 1;
}
static const int cLeftAdd[16] = {0,0,0,0,4,0,0,0,4,0,0,0,4,0,0,0};
static const int cRightAdd[16] = {1,1,1,-3,1,1,1,-3,1,1,1,-3,1,1,1,-3};
GLTransform3D::GLTransform3D(const GLTransform3D &left, const GLTransform3D &right) {
    const float *l = left._transformMat;
    const float *r = right._transformMat;
    int numLeft = 0;
    int numRight = 0;
    for (int i = 0; i < 15; ++i) {
        _transformMat[i] = l[numLeft] * r[numRight]
                           + l[numLeft + 1] * r[numRight + 4]
                           + l[numLeft + 2] * r[numRight + 8]
                           + l[numLeft + 3] * r[numRight + 12];
        numLeft += cLeftAdd[i];
        numRight += cRightAdd[i];
    }
}

void GLTransform3D::setScale(float x, float y) {
    _transformMat[0] = x;
    _transformMat[5] = y;
}
void GLTransform3D::setScale(float x, float y, float z) {
    _transformMat[0] = x;
    _transformMat[5] = y;
    _transformMat[10] = z;
}
void GLTransform3D::setPosition(float x, float y) {
    _transformMat[12] = x;
    _transformMat[13] = y;
}
void GLTransform3D::move(float x, float y) {
    _transformMat[12] += x;
    _transformMat[13] += y;
}

void GLTransform3D::setPosition(float x, float y, float z) {
    _transformMat[12] = x;
    _transformMat[13] = y;
    _transformMat[14] = z;
}

GLTransform3D GLTransform3D::operator * (const GLTransform3D&right) {
    return GLTransform3D(*this, right);
}
