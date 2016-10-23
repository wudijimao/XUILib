//
//  GLTransform3D.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/31.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "../stdafx.hpp"

struct SIMPLEDIRECTUI_API GLTransform3D {
public:
    GLTransform3D();
    GLTransform3D(const GLTransform3D &left, const GLTransform3D &right);
	float _transformMat[16];
    void setScale(float x, float y);
    void setScale(float x, float y, float z);
    void setPosition(float x, float y);
    void move(float x, float y);
    void setPosition(float x, float y, float z);
    GLTransform3D operator * (const GLTransform3D&right);
};