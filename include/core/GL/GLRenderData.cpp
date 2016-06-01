//
//  GLRenderData.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "GLRenderData.hpp"

namespace XDUILib {
    GLProgram GLRenderSquareData::sProgram;
    
    GLProgram GLRenderNineGridData::sProgram;
    
    
    
    GLRenderData::~GLRenderData(){};
    GLRenderData::GLRenderData() {
    }
    GLuint _vectexArrayObject;
    GLRenderDataType GLRenderData::Type() {
        return GLRenderDataType::UnKnown;
    }
    void GLRenderData::setPosition(const XResource::XDisplayPoint &point) {
        _transform.setPosition(point.X(), point.Y());
    }
    void GLRenderData::move(const XResource::XDisplayPoint &point) {
        _transform.move(point.X(), point.Y());
    }
    
    
    GLRenderDataType GLRenderSquareData::Type() {
        return GLRenderDataType::Square;
    }
    
    void GLRenderSquareData::setClips(bool clips) {
        mIsClips = clips;
    }
    
    void GLRenderSquareData::setClipsBound(const XResource::XRect &rect) {
        _clipsX1 = rect.X();
        _clipsX2 = _clipsX1 + rect.Width();
        _clipsY1 = rect.Y();
        _clipsY2 = _clipsY1 + rect.Height();
    }
    
    void GLRenderSquareData::setMaskImage(const std::shared_ptr<XResource::IXImage> image) {
        if (image.get() != nullptr) {
            _maskTextureId = GLTextureManager::sharedInstance().getTextureID(image);
        } else {
            _maskTextureId = 0;
        }
    }
    
    void GLRenderSquareData::setSquare(const XResource::XRect &rect) {
        setPosition(rect.point());
        _square[0] = 0;
        _square[1] = 0;
        _square[2] = 0.5;
        
        _square[6] = rect.Width();
        _square[7] = 0;
        _square[8] = 0.5;
        
        _square[9] = rect.Width();
        _square[10] = rect.Height();
        _square[11] = 0.5;
        
        _square[3] = 0;
        _square[4] = rect.Height();
        _square[5] = 0.5;
    }
    
    void GLRenderSquareData::initWithRect(const XResource::XRect &rect, const XResource::XColor &color, const std::shared_ptr<XResource::IXImage> &image) {
        setSquare(rect);
        
        _texturePos[0] = 0;
        _texturePos[1] = 0;
        _texturePos[2] = 0;
        _texturePos[3] = 1;
        _texturePos[4] = 1;
        _texturePos[5] = 0;
        _texturePos[6] = 1;
        _texturePos[7] = 1;
        
        if (image.get() != nullptr) {
            _textureId = GLTextureManager::sharedInstance().getTextureID(image);
            if (image->pixelFormat() == XResource::XImagePixelFormat::Gray) {
                mIsAlphaTexture = true;
            }
        }
        
        GLfloat glColor[4] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };
        size_t size = sizeof(glColor);
        for (int i = 0; i < 4; ++i) {
            memcpy(&_color[i * 4], glColor, size);
        }
        buildVAO();
    }
    void GLRenderSquareData::render() {
        GLRenderSquareData::sProgram.enable();
        sProgram.setUniformMatrix4fv("viewMat", 1, (GLfloat*)&_transform._transformMat);
        if (mIsClips) {
            GLRenderSquareData::sProgram.setUniformValue("uClipsBounds", _clipsX1, _clipsX2, _clipsY1, _clipsY2);
        }
        GLRenderSquareData::sProgram.setUniformValue("uIsClipsToBounds", mIsClips);
        
        if(_maskTextureId > 0) {
            glActiveTexture(GL_ACTIVE_TEXTURE - 2);
            glBindTexture(GL_TEXTURE_2D, _maskTextureId);
            GLRenderSquareData::sProgram.setUniformValue("s_mask_texture", GL_ACTIVE_TEXTURE - GL_TEXTURE0 - 2);
            GLRenderSquareData::sProgram.setUniformValue("uHasMaskImage", true);
        } else {
            GLRenderSquareData::sProgram.setUniformValue("uHasMaskImage", false);
        }
        
        if (_textureId > 0) {
            glActiveTexture(GL_ACTIVE_TEXTURE - 1);
            glBindTexture(GL_TEXTURE_2D, _textureId);
            GLRenderSquareData::sProgram.setUniformValue("s_texture", GL_ACTIVE_TEXTURE - GL_TEXTURE0 - 1);
            GLRenderSquareData::sProgram.setUniformValue("useTexture", true);
            GLRenderSquareData::sProgram.setUniformValue("uIsTextureAlpha", mIsAlphaTexture);
        }
        else {
            GLRenderSquareData::sProgram.setUniformValue("useTexture", false);
        }
        //glEnableVertexAttribArray(0);
        //glEnableVertexAttribArray(1);
        glBindVertexArray(_vectexArrayObject);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
        // Draw
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    
    GLRenderSquareData::~GLRenderSquareData() {
        glDeleteBuffers(3, bufObjects);
        glDeleteVertexArrays(1, &_vectexArrayObject);
    }
    
    void GLRenderSquareData::buildVAO() {
        GLRenderSquareData::sProgram.enable();
        
        //must first create on gl 4 core
        glGenVertexArrays(1, &_vectexArrayObject);
        glBindVertexArray(_vectexArrayObject);
        
        GLuint inPos = sProgram.getAttributeIndex("inPos");
        GLuint vTexCoord = sProgram.getAttributeIndex("vTexCoord");
        GLuint inColor = sProgram.getAttributeIndex("inColor");
        
        glGenBuffers(3, bufObjects);
        
        glBindBuffer(GL_ARRAY_BUFFER, bufObjects[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_square), _square, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)inPos, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        
        
        glBindBuffer(GL_ARRAY_BUFFER, bufObjects[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_texturePos), _texturePos, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)vTexCoord, 2, GL_FLOAT, GL_TRUE, 0, nullptr);
        
        
        glBindBuffer(GL_ARRAY_BUFFER, bufObjects[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_color), _color, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)inColor, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        
        //            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufObjects[3]);
        //            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices) , _indices, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(inPos);
        glEnableVertexAttribArray(vTexCoord);
        glEnableVertexAttribArray(inColor);
        
        glBindVertexArray(0);
        /*glDisableVertexAttribArray(0);
         glDisableVertexAttribArray(1);
         glDisableVertexAttribArray(2);*/
    }
    
    GLRenderDataType GLRenderNineGridData::Type() {
        return GLRenderDataType::NineGrid;
    }
    
    void GLRenderNineGridData::initWithRect(const XResource::XRect &rect, const std::shared_ptr<XResource::XStretchableImage> &image) {
        setPosition(rect.point());
        for (auto square : _square) {
            square.z = 0.5f;
        }
        double imageWidth = image->image()->size().Width();
        double imageHeight = image->image()->size().Height();
        GLfloat y = 0;
        GLfloat v = 0;
        int i = 0;
        while (i < 4) {
            _texturePos[i].v = v;
            _square[i++].y = y;
        }
        y = image->stretchEdge().top();
        v = image->stretchEdge().top() / imageHeight;
        while (i < 8) {
            _texturePos[i].v = v;
            _square[i++].y = y;
        }
        y = rect.Height() - image->stretchEdge().bottom();
        v = (imageHeight - image->stretchEdge().bottom()) / imageHeight;
        while (i < 12) {
            _texturePos[i].v = v;
            _square[i++].y = y;
        }
        y = rect.Height();
        v = 1.0f;
        while (i < 16) {
            _texturePos[i].v = v;
            _square[i++].y = y;
        }
        
        
        GLfloat x = 0;
        GLfloat u = 0;
        i = 0;
        while (i < 16) {
            _texturePos[i].u = u;
            _square[i].x = x;
            i += 4;
        }
        x = image->stretchEdge().left();
        u = image->stretchEdge().left() / imageWidth;
        i = 1;
        while (i < 16) {
            _texturePos[i].u = u;
            _square[i].x = x;
            i += 4;
        }
        x = rect.Width() - image->stretchEdge().right();
        u = (imageWidth - image->stretchEdge().right()) / imageWidth;
        i = 2;
        while (i < 16) {
            _texturePos[i].u = u;
            _square[i].x = x;
            i += 4;
        }
        x = rect.Width() ;
        u = 1;
        i = 3;
        while (i < 16) {
            _texturePos[i].u = u;
            _square[i].x = x;
            i += 4;
        }
        
        GLubyte index = 0;
        int lineIndex = 0;
        for(int i = 0; i < 54; i += 6, ++index, ++lineIndex) {
            indices[i] = index;
            indices[i+1] = index+4;
            indices[i+2] = index+1;
            indices[i+3] = index+1;
            indices[i+4] = index+4;
            indices[i+5] = index+5;
            if(lineIndex >= 2) {
                lineIndex = -1;
                ++index;
            }
        }
        
        if (image.get() != nullptr) {
            _textureId = GLTextureManager::sharedInstance().getTextureID(image->image());
        }
        buildVAO();
    }
    void GLRenderNineGridData::render() {
        sProgram.enable();
        sProgram.setUniformMatrix4fv("viewMat", 1, (GLfloat*)&_transform._transformMat);
        sProgram.setUniformValue("uIsClipsToBounds", false);
        
        if (_textureId > 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _textureId);
            GLRenderSquareData::sProgram.setUniformValue("s_texture", GL_TEXTURE0);
            
            GLRenderSquareData::sProgram.setUniformValue("useTexture", true);
            GLRenderSquareData::sProgram.setUniformValue("uIsTextureAlpha", false);
        }
        else {
            return;
            //GLRenderSquareData::sProgram.setUniformValue("useTexture", false);
        }
        glBindVertexArray(_vectexArrayObject);
        glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_BYTE, 0);
    }
    
    void GLRenderNineGridData::buildVAO() {
        GLRenderSquareData::sProgram.enable();
        
        //must first create on gl 4 core
        glGenVertexArrays(1, &_vectexArrayObject);
        glBindVertexArray(_vectexArrayObject);
        
        GLuint inPos = sProgram.getAttributeIndex("inPos");
        GLuint vTexCoord = sProgram.getAttributeIndex("vTexCoord");
        
        glGenBuffers(3, bufObjects);
        
        glBindBuffer(GL_ARRAY_BUFFER, bufObjects[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_square), _square, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)inPos, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        
        
        glBindBuffer(GL_ARRAY_BUFFER, bufObjects[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_texturePos), _texturePos, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)vTexCoord, 2, GL_FLOAT, GL_TRUE, 0, nullptr);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufObjects[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) , indices, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(inPos);
        glEnableVertexAttribArray(vTexCoord);
        
        glBindVertexArray(0);
        /*glDisableVertexAttribArray(0);
         glDisableVertexAttribArray(1);
         glDisableVertexAttribArray(2);*/
    }

}