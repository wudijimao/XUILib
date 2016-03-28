//
//  GLRenderData.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "stdafx.hpp"
#include "../../res/XResource.hpp"
#include "GLHeaders.h"

namespace XDUILib {
    class GLTexture {
    public:
        std::shared_ptr<XResource::XImage> Img() {
            return _img;
        }
        int glTextureIndex() {
            return _glTextureIndex;
        }
    protected:
        GLTexture(std::shared_ptr<XResource::XImage> img, int glTextureIndex) {
            _img = img;
            _glTextureIndex = glTextureIndex;
        }
    private:
        std::shared_ptr<XResource::XImage> _img;
        int _glTextureIndex;
    };
    class GLTextureManager {
        
    };
    
    enum class GLRenderDataType {
        UnKnown,
        Square,
    };
    class GLRenderData {
    public:
        GLuint _vectexArrayObject;
        virtual GLRenderDataType Type() {
            return GLRenderDataType::UnKnown;
        }
    };
    class GLRenderSquareData : public GLRenderData {
    public:
        GLfloat _square[12];
        GLfloat _texturePos[8];
        GLfloat _color[16];
        virtual GLRenderDataType Type() {
            return GLRenderDataType::Square;
        }
        void initWithRect(const XResource::XRect &rect, const XResource::XColor &color, const XResource::XImage &image) {
            _square[0] = rect.X();
            _square[1] = rect.Y();
            _square[2] = 0.5;
            
            _square[6] = rect.X() + rect.Width();
            _square[7] = rect.Y();
            _square[8] = 0.5;
            
            _square[9] = rect.X() + rect.Width();
            _square[10] = rect.Y() + rect.Height();
            _square[11] = 0.5;
            
            _square[3] = rect.X();
            _square[4] = rect.Y() + rect.Height();
            _square[5] = 0.5;
            
            GLfloat glColor[4] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };
            size_t size = sizeof(glColor);
            for (int i = 0; i < 4; ++i) {
                memcpy(&_color[i * 4], glColor, size);
            }
            buildVAO();
        }
    private:
        void buildVAO() {
            GLuint bufObjects[3];
            glGenBuffers(3, bufObjects);
            
            glBindBuffer(GL_ARRAY_BUFFER, bufObjects[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_square), _square, GL_STATIC_DRAW);
            
            glBindBuffer(GL_ARRAY_BUFFER, bufObjects[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_texturePos), _texturePos, GL_STATIC_DRAW);
            
            glBindBuffer(GL_ARRAY_BUFFER, bufObjects[2]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_color), _color, GL_STATIC_DRAW);
            
            glGenVertexArrays(1, &_vectexArrayObject);
            glBindVertexArray(_vectexArrayObject);
            glBindBuffer(GL_ARRAY_BUFFER, bufObjects[0]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glBindBuffer(GL_ARRAY_BUFFER, bufObjects[1]);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, nullptr);
            glBindBuffer(GL_ARRAY_BUFFER, bufObjects[2]);
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glBindVertexArray(0);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
        }
    };
}




