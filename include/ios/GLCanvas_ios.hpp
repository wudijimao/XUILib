#pragma once
#include "../core/stdafx.hpp"
#include "../core/GLCanvas.hpp"
#import "XDUIViewController.h"


class GLCanvas_ios : public GLCanvas
{
public:
    ~GLCanvas_ios() {
    }
    virtual XResource::XSize GetSize() {
        return XResource::XSize(100, 100);
    }
    bool init(CAEAGLLayer* drawable) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        [EAGLContext setCurrentContext:_context];
        
//        glGenRenderbuffers(1, &_renderBuffer);
//        glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
//        [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:drawable];
        if(!this->InitGLProgram()) {
            assert(false);
            return false;
        }
//        if (!this->InitFrameBuffer()) {
//            assert(false);
//            return false;
//        }
        return true;
    }
    bool Present() {
//        if ([EAGLContext currentContext] == _context) {
//            [EAGLContext setCurrentContext:nil];
//        }
        this->GLCanvas::Present();
        //[_context presentRenderbuffer:_renderBuffer];
        return true;
    }
public:
    EAGLContext *_context; //ARC in ios
};
