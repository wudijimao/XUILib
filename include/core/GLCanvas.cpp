//
//  GLCanvas.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "GLCanvas.hpp"
#include "XResManager.hpp"

//temp
#include <iostream>

RenderType GLCanvas::GetType()
{
    return RenderType::OPENGL;
}
void GLCanvas::pushRenderData(XDUILib::GLRenderData *data) {
    _needRenderDatas.push_back(data);
}

bool GLCanvas::InitGLProgram() {
    std::shared_ptr<XBundle> bundle = XBundle::libBundle();
	auto vsh = bundle->pathForResource("shader/VertexShader", "vsh");
	auto fsh = bundle->pathForResource("shader/FragmentShader", "fsh");
    return _program.initWithFilePath(vsh.UTF8CStr() , fsh.UTF8CStr());
}

bool GLCanvas::InitFrameBuffer() {
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderBuffer);
    //声明并赋值一个GL枚举变量，赋值为检测GL_FRAMEBUFFER状态的返回值，
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //如果状态不完全
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }
    return true;
}

void GLCanvas::enableGLSettings() {
    //see docoment https://www.opengl.org/wiki/Blending
    glEnable(GL_BLEND);
    //glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void GLCanvas::clear() {
	for (auto data : _needRenderDatas)
	{
		delete data;
	}
    _needRenderDatas.clear();
}


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
	300, 200, 0.5 };

GLfloat transformMat[] = {
	1.0f / 150.0f, 0, 0, 0,
	0, 1.0f / 150.0f, 0, 0,
	0, 0, 1, 0,
	-1, -1, 0, 1, };

bool GLCanvas::Present() {
    glViewport(0, 0, _size.Width(), _size.Height());
//    glClearColor(1, 1, 1, 1.0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto data : _needRenderDatas) {
        switch(data->Type()) {
            case XDUILib::GLRenderDataType::Square: {
                _program.enable();
                
                GLuint textureId = ((XDUILib::GLRenderSquareData*)data)->_textureId;
                if (textureId < 10) {
                    glActiveTexture(GL_ACTIVE_TEXTURE - 1);
                    glBindTexture(GL_TEXTURE_2D, textureId);
                    _program.setUniformValue("s_texture", GL_ACTIVE_TEXTURE - GL_TEXTURE0 - 1);
                    
                    _program.setUniformValue("useTexture", true);
                } else {
                    _program.setUniformValue("useTexture", false);
                }
                
                glUniformMatrix4fv(0, 1, true, (GLfloat*)&_transformMat);
                //glEnableVertexAttribArray(0);
                //glEnableVertexAttribArray(1);
                glBindVertexArray(data->_vectexArrayObject);
                
                // Draw
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                std::cout << "present HOme" << std::endl;
            }
                break;
            default:
                break;
        }
    }
	return true;

	GLuint vectexArrayObject;

	GLuint bufObjects[2];
	glGenBuffers(2, bufObjects);

	glBindBuffer(GL_ARRAY_BUFFER, bufObjects[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, bufObjects[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturePos), texturePos, GL_STATIC_DRAW);


	glGenVertexArrays(1, &vectexArrayObject);
	glBindVertexArray(vectexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, bufObjects[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, bufObjects[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glUniformMatrix4fv(0, 1, true, (GLfloat*)&transformMat);

	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	glBindVertexArray(vectexArrayObject);

	// Draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	GLuint error = glGetError();

    return true;
}


