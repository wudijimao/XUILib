#pragma once
#include "../core/stdafx.hpp"
#include "../core/IXCanvas.hpp"


#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import "GLView.h"
#import "ResourceHelper.h"

GLfloat texturePos[] = {
    0.00, 0.00,
    1, 0,
    0, 1,
    1, 1
};

GLfloat square[] = {
    0, 0, 0.5,
    300, 0, 0.5,
    0, 300, 0.5,
    300, 200, 0.5};

GLfloat transformMat[] = {
    1.0f/150.0f, 0, 0, 0,
    0, 1.0f/150.0f, 0, 0,
    0, 0, 1, 0,
    -1, -1, 0, 1,};

class GLCanvas : public IXCanvas {
public:
    virtual RenderType GetType()
    {
        return RenderType::OPENGL;
    }
protected:
    GLuint _renderBuffer;
    GLuint _framebuffer;
    GLuint _program;
    GLuint _vectexArrayObject;
protected:
    XResource::XSize _size;
    bool InitVAO() {
        glGenFramebuffers(1, &_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderBuffer);
        glViewport(0, 0, _size.Width(), _size.Height());
        glClearColor(0, 1, 0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //声明并赋值一个GL枚举变量，赋值为检测GL_FRAMEBUFFER状态的返回值，
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        //如果状态不完全
        if(status != GL_FRAMEBUFFER_COMPLETE) {
            return false;
        }

        GLuint bufObjects[2];
        glGenBuffers(2, bufObjects);
        
        glBindBuffer(GL_ARRAY_BUFFER, bufObjects[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, bufObjects[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texturePos), texturePos, GL_STATIC_DRAW);
        
        
        glGenVertexArrays(1, &_vectexArrayObject);
        glBindVertexArray(_vectexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, bufObjects[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, bufObjects[1]);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, nullptr);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
        return true;
    }
    bool InitGLProgram(const char *vertexShaderText, const char *fragmentShaderText) {
        if (vertexShaderText == nullptr || fragmentShaderText == nullptr) {
            return false;
        }
        GLint compileResult;
        
        // Create and compile vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderText, NULL);
        glCompileShader(vertexShader);
        
        glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&compileResult);
        if (GL_FALSE == compileResult)
        {
            return false;
        }
        
        
        // Create and compile fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderText, NULL);
        glCompileShader(fragmentShader);
        
        glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&compileResult);
        if (GL_FALSE == compileResult)
        {
            GLint logLen;
            //得到编译日志长度
            glGetShaderiv(fragmentShader,GL_INFO_LOG_LENGTH,&logLen);
            if (logLen > 0)
            {
                char *log = (char *)malloc(logLen);
                GLsizei written;
                //得到日志信息并输出
                glGetShaderInfoLog(fragmentShader,logLen,&written,log);
                free(log);//释放空间
            }
            return false;
        }
        
        // Create and link program
        _program = glCreateProgram();
        glAttachShader(_program, vertexShader);
        glAttachShader(_program, fragmentShader);
        glLinkProgram(_program);
        
        glGetProgramiv(_program, GL_LINK_STATUS, &compileResult);
        if (GL_FALSE == compileResult) {
            GLint logLen;
            //得到编译日志长度
            glGetShaderiv(fragmentShader,GL_INFO_LOG_LENGTH,&logLen);
            if (logLen > 0)
            {
                char *log = (char *)malloc(logLen);
                GLsizei written;
                //得到日志信息并输出
                glGetShaderInfoLog(fragmentShader,logLen,&written,log);
                free(log);//释放空间
            }
            return false;
        }
        
        // Use program
        glUseProgram(_program);
        return true;
    }
    void TestPlay() {
//        GLint location = glGetUniformLocation(_program, "s_texture");
//        GLuint textureId = loadTexture(@"test");
//        
//        glActiveTexture(GL_ACTIVE_TEXTURE - 1);
//        glBindTexture(GL_TEXTURE_2D, textureId);
//        glUniform1i(location, GL_ACTIVE_TEXTURE - GL_TEXTURE0 - 1);
        
        glUniformMatrix4fv(0, 1, true, (GLfloat*)&transformMat);
        
        //glEnableVertexAttribArray(0);
        //glEnableVertexAttribArray(1);
        glBindVertexArray(_vectexArrayObject);
        
        // Draw
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
};

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
        
        this->InitVAO();
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
        if (!this->InitVAO()) {
            return false;
        }
        return true;
    }
    bool Present() {
        this->TestPlay();
        [_context presentRenderbuffer:_renderBuffer];
        return true;
    }
private:
    EAGLContext *_context; //ARC in ios
};
