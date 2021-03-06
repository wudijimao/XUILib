#pragma once
#include "../../include/core/stdafx.hpp"
#include "../../include/core/GLCanvas.hpp"
#import "XDUIViewController.h"


class GLCanvas_ios : public GLCanvas
{
public:
    ~GLCanvas_ios() {
    }
    virtual XResource::XSize GetSize() {
        return _size;
    }
    bool init(CAEAGLLayer* drawable) {
        updateSize(drawable);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        if (_context == nil) {
            _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        }
        [EAGLContext setCurrentContext:_context];
        
        glGenRenderbuffers(1, &_renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
        //glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, _size.Width() * 2, _size.Height() + 2);
        
        NSMutableDictionary *dict = [drawable.drawableProperties mutableCopy];
        dict[kEAGLDrawablePropertyColorFormat] = kEAGLColorFormatSRGBA8;
        drawable.drawableProperties = dict;
        if(![_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:drawable]) {
            assert(false);
            return false;
        }
        if(!this->InitGLProgram()) {
            assert(false);
            return false;
        }
        if (!this->InitFrameBuffer()) {
            assert(false);
            return false;
        }
        this->enableGLSettings();
        return true;
    }
    void updateSize(CAEAGLLayer* drawable) {
        _size.Width(drawable.frame.size.width);
        _size.Height(drawable.frame.size.height);
        setSize(_size, drawable.contentsScale);
    }
    bool Present() {
        this->GLCanvas::Present();
        [_context presentRenderbuffer:_renderBuffer];
        return true;
    }
public:
    EAGLContext *_context; //ARC in ios
};


