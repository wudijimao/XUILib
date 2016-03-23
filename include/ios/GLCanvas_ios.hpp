#pragma once
#include "../core/stdafx.hpp"
#include "../core/GLCanvas.hpp"

#import "GLView.h"
#import "ResourceHelper.h"


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
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        [EAGLContext setCurrentContext:_context];
        
        glGenRenderbuffers(1, &_renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
        [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)view.layer];
        
        NSString *path = [ResourceHelper pathForResource:@"shader/VertexShader" ofType:@"vsh"];
        NSString *vertexShaderSource = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil];
        const char *vertexShaderSourceCString = [vertexShaderSource cStringUsingEncoding:NSUTF8StringEncoding];
        // Read fragment shader source
        path = [ResourceHelper pathForResource:@"shader/FragmentShader" ofType:@"fsh"];
        NSString *fragmentShaderSource = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil];
        const char *fragmentShaderSourceCString = [fragmentShaderSource cStringUsingEncoding:NSUTF8StringEncoding];
        
        if(!this->InitGLProgram(vertexShaderSourceCString, fragmentShaderSourceCString)) {
            return false;
        }
        if (!this->InitFrameBuffer()) {
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
