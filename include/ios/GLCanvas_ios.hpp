#pragma once
#include "../core/stdafx.hpp"
#include "../core/GLCanvas.hpp"

#import "GLView.h"

class GLCanvas_ios : public GLCanvas
{
public:
    ~GLCanvas_ios() {
    }
    virtual XResource::XSize GetSize() {
        return XResource::XSize(100, 100);
    }
    bool InitWithGLView(GLView *view) {
        _size.Width(view.frame.size.width);
        _size.Height(view.frame.size.height);
        setSize(_size);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        [EAGLContext setCurrentContext:_context];
        
        glGenRenderbuffers(1, &_renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
        [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)view.layer];
        
        if(!this->InitGLProgram()) {
            assert(false);
            return false;
        }
        if (!this->InitFrameBuffer()) {
            assert(false);
            return false;
        }
        return true;
    }
    bool Present() {
        this->GLCanvas::Present();
        [_context presentRenderbuffer:_renderBuffer];
        return true;
    }
private:
    EAGLContext *_context; //ARC in ios
};
