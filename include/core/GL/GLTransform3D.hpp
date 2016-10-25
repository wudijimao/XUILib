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


struct SIMPLEDIRECTUI_API Vec4 {
    float _vec[4];
    Vec4() {
        memset(_vec, 0, sizeof(_vec));
    }
    Vec4(const Vec4 &in, const GLTransform3D &transform) {
        _vec[0] = in._vec[0] * transform._transformMat[0]
                + in._vec[1] * transform._transformMat[4]
                + in._vec[2] * transform._transformMat[8]
                + in._vec[3] * transform._transformMat[12];
        _vec[1] = in._vec[0] * transform._transformMat[1]
                + in._vec[1] * transform._transformMat[5]
                + in._vec[2] * transform._transformMat[9]
                + in._vec[3] * transform._transformMat[13];
        _vec[2] = in._vec[0] * transform._transformMat[2]
                + in._vec[1] * transform._transformMat[6]
                + in._vec[2] * transform._transformMat[10]
                + in._vec[3] * transform._transformMat[14];
        _vec[3] = in._vec[0] * transform._transformMat[3]
                + in._vec[1] * transform._transformMat[7]
                + in._vec[2] * transform._transformMat[11]
                + in._vec[3] * transform._transformMat[15];
    }
};