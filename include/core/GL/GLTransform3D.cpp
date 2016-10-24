//
//  GLTransform3D.cpp
//  XDUILib
//
//  Created by ximiao on 16/5/31.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "GLTransform3D.hpp"
#include <math.h>


GLTransform3D::GLTransform3D() {
	memset(&_transformMat, 0, sizeof(_transformMat));
    _transformMat[0] = 1;
    _transformMat[5] = 1;
    _transformMat[10] = 1;
    _transformMat[15] = 1;
}
static const int cLeftAdd[16] = {0,0,0,4,0,0,0,4,0,0,0,4,0,0,0,4};
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
    _transformMat[15] = 1;
}

inline void transfromDegree(float &degree) {
    degree = (degree / 180) * 3.14159265359;
}

GLTransform3D::GLTransform3D(float viewDegree, float aspect, float zn, float zf)
{
    transfromDegree(viewDegree);
    memset(_transformMat, 0, sizeof(_transformMat));
    _transformMat[0] = 1 / (tan(viewDegree * 0.5f) *aspect) ;
    _transformMat[5] = 1 / tan(viewDegree * 0.5f) ;
    _transformMat[10] = zf / (zf - zn) ;
    _transformMat[11] = 1.0f;
    _transformMat[14] = (zn * zf) / (zn - zf);
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



void GLTransform3D::setRotation(float z) {
    transfromDegree(z);
    _transformMat[5] = _transformMat[0] = cosf(z);
    _transformMat[1] = sinf(z);
    _transformMat[4] = -_transformMat[1];
}
void GLTransform3D::setRotationX(float x) {
    transfromDegree(x);
    _transformMat[10] = _transformMat[5] = cosf(x);
    _transformMat[6] = sinf(x);
    _transformMat[9] = - _transformMat[6];
}
void GLTransform3D::setRotationY(float y) {
    transfromDegree(y);
    _transformMat[10] = _transformMat[0] = cosf(y);
    _transformMat[8] = sinf(y);
    _transformMat[2] = - _transformMat[8];
}

GLTransform3D GLTransform3D::operator * (const GLTransform3D&right) {
    return GLTransform3D(*this, right);
}



