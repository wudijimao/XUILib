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
    
    //y方向的视角，纵横比，近剪裁平面到原点的距离及远剪裁平面到原点的距离
    GLTransform3D(float viewDegree, float aspect, float zn, float zf);
	float _transformMat[16];
    void setPosition(float x, float y);
    void move(float x, float y);
    //以下设置冲突  如果想实现分别不同轴的旋转或旋转和缩放同时进行 需要两个truansform相乘？？
    void setScale(float x, float y);
    void setScale(float x, float y, float z);
    void setPosition(float x, float y, float z);
    void setRotation(float z);
    void setRotationX(float x);
    void setRotationY(float y);
    GLTransform3D operator * (const GLTransform3D&right);
};