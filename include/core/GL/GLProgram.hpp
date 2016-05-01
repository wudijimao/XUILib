//
//  GLProgram.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/27.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include "../stdafx.hpp"
#include "GLHeaders.h"
#include "../../res/XResource.hpp"

class GLProgram {
public:
    bool init(const char *vertexShaderText, const char *fragmentShaderText);
    bool initWithFilePath(const char *vFilePath, const char *fFilePath);
    bool setUniformValue(const char *uniformName, const XResource::XColor &color);
    bool setUniformValue(const char *uniformName, bool b);
    bool setUniformValue(const char *uniformName, GLint i);
    bool enable();
	GLuint getAttributeIndex(const char *name);
private:
    GLuint _program;
    
private:
    bool checkShaderCompilState(GLuint shader);
    bool checkProgramCompilState(GLuint program);
};
