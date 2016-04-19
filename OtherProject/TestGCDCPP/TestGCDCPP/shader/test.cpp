//
//  test.cpp
//  TestGCDCPP
//
//  Created by ximiao on 16/2/1.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#include <stdio.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>


struct XColor {
    char r = '\0';
    char g = '\0';
    char b = '\0';
    char a = '\0';
};


void dra() {
    int width = 300;
    int height = 500;
    
    GLuint renderBuffer;
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    //[context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)glView.layer];
    glRenderbufferStorage(GL_RENDERBUFFER, GL_SRGB8_ALPHA8, 300, 500);
    GLenum err = glGetError();
    
    //    //创建一个OpenGL ES用的无符号整形缓存,名为depthRenderbuffer
    //    GLuint depthRenderbuffer;
    //    //从depthRenderbuffer的地址起，分配一个渲染缓存的内存，（1个，地址）
    //    glGenRenderbuffers(1, &depthRenderbuffer);
    //    //把depthRenderbuffer绑定在OpenGL ES的渲染缓存GL_RENDERBUFFER上
    //    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    //    //设定渲染缓存的存储类型（GL_DEPTH_COMPONENT16、宽、高）
    //    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 100, 100);
    //    //把GL_RENDERBUFFER里的depthRenderbuffer附在GL_FRAMEBUFFER的GL_DEPTH_ATTACHMENT（深度附着点）上
    //    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    //    err = glGetError();
    
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);
    
    //声明并赋值一个GL枚举变量，赋值为检测GL_FRAMEBUFFER状态的返回值，
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //如果状态不完全
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        //显示语句：创建完全的帧缓存对象失败，状态是...
        NSLog(@"failed to make complete framebuffer object %x", status);
    }
    
    
    glViewport(0, 0, width, height);
    
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    // Read vertex shader source
    NSString *vertexShaderSource = [NSString stringWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"VertexShader" ofType:@"vsh"] encoding:NSUTF8StringEncoding error:nil];
    const char *vertexShaderSourceCString = [vertexShaderSource cStringUsingEncoding:NSUTF8StringEncoding];
    
    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceCString, NULL);
    glCompileShader(vertexShader);
    
    
    GLint compileResult;
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&compileResult);
    if (GL_FALSE == compileResult)
    {
        int a = 1;
    }
    
    // Read fragment shader source
    NSString *fragmentShaderSource = [NSString stringWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"FragmentShader" ofType:@"fsh"] encoding:NSUTF8StringEncoding error:nil];
    const char *fragmentShaderSourceCString = [fragmentShaderSource cStringUsingEncoding:NSUTF8StringEncoding];
    
    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCString, NULL);
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
            NSLog(@"Log:%s", log);
            free(log);//释放空间
        }
    }
    
    
    // Create and link program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    // Use program
    glUseProgram(program);
    
    GLint location = glGetUniformLocation(program, "s_texture");
    
    
    // Define geometry
    GLfloat square[] = {
        -10.5, -10.5, 1.0,
        10.5, -10.5, 1.0,
        -10.5, 10.5, 1.0,
        10.5, 10.5, 1.0};
    
    //Send geometry to vertex shader
    //const char *aPositionCString = [@"a_position" cStringUsingEncoding:NSUTF8StringEncoding];
    //GLuint aPosition = glGetAttribLocation(program, aPositionCString);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, square);
    
    GLfloat texturePos[] = {
        0, 0,
        0, 1,
        1, 1,
        1, 0
    };
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, texturePos);
    
    XColor *textureData = new XColor[width*height];
    for (int i = 0; i < width*height; ++i) {
        textureData[i].r = '\255';
        textureData[i].g = '\255';
        textureData[i].b = '\0';
        textureData[i].a = '\255';
    }
    
    GLuint textureId;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glUniform1i(location, 0);
    
    
    CATransform3D transform = CATransform3DMakeTranslation(10.0f, 10.0f, 10.0f);
    GLfloat transformMat[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,};
    glUniformMatrix4fv(0, 1, true, (GLfloat*)&transformMat);
    
    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    GLuint error = glGetError();
    
    //[context presentRenderbuffer:GL_RENDERBUFFER];
    
    XColor *pixels = new XColor[width*height];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}