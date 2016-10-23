//
//  GLHeaders.h
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include "../../core/stdafx.hpp"
#ifdef TARGET_OS_IPHONE
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import <OpenGLES/gltypes.h>
#endif

#ifdef TARGET_OS_MSWINDOWS
#define GLEW_STATIC
#include "GL\glew.h"
#endif // TARGET_OS_MSWINDOWS


#ifdef TARGET_OS_ANDROID
    #include <EGL/egl.h>
    #include <GLES/gl.h>
    #if DYNAMIC_ES3
        #include "gl3stub.h"
    #else
        #include <GLES3/gl3.h>
    #endif
#endif