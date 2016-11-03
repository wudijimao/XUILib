//
//  GLRenderData.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "GLRenderData.hpp"
#include "../GLRender.hpp"



namespace XDUILib {
    GLProgram GLRenderSquareData::sProgram;
    
    GLRenderData::~GLRenderData(){};
    GLRenderData::GLRenderData(GLRender *render) : _belongRender(render) {
    }
    GLuint _vectexArrayObject;
    GLRenderDataType GLRenderData::Type() {
        return GLRenderDataType::UnKnown;
    }
//    void GLRenderData::setPosition(const XResource::XDisplayPoint &point) {
//        _transform.setPosition(point.X(), point.Y());
//    }
//    void GLRenderData::move(const XResource::XDisplayPoint &point) {
//        _transform.move(point.X(), point.Y());
//    }
    
    
    GLRenderDataType GLRenderSquareData::Type() {
        return GLRenderDataType::Square;
    }
    
    void GLRenderSquareData::setClips(bool clips) {
        mIsClips = clips;
    }
    
    void GLRenderSquareData::setMaskImage(const std::shared_ptr<XResource::IXImage> image) {
        if (image.get() != nullptr) {
            _maskTextureId = GLTextureManager::sharedInstance().getTextureID(image);
        } else {
            _maskTextureId = 0;
        }
    }


    GLRenderSquareData::GLRenderSquareData(GLRender *render) : GLRenderData(render) {
    }
    
    void GLRenderSquareData::setSquare(const XResource::XRect &rect) {
//setPosition(rect.point()); //之后删除transform  直接写入到_square顶点数据中， 移动不再移动单个RenderData 而是移动整体Render的transform
        _square[0] = rect.X();
        _square[1] = rect.Y();
        _square[2] = 0;
        
        _square[6] = rect.rX();
        _square[7] = rect.Y();
        _square[8] = 0;
        
        _square[9] = rect.rX();
        _square[10] = rect.bY();
        _square[11] = 0;
        
        _square[3] = rect.X();
        _square[4] = rect.bY();
        _square[5] = 0;
    }
    
    void GLRenderSquareData::initWithRect(const XResource::XRect &rect, const XResource::XColor &color, const std::shared_ptr<XResource::IXImage> &image) {
        setSquare(rect);
//        _transform = _transform * _belongRender->getRenderDataPovider().rd_Transform();
//        
        _texturePos[0] = 0.0f;
        _texturePos[1] = 0.0f;
        _texturePos[2] = 0.0f;
        _texturePos[3] = 1.0f;
        _texturePos[4] = 1.0f;
        _texturePos[5] = 0.0f;
        _texturePos[6] = 1.0f;
        _texturePos[7] = 1.0f;
        
        if (image.get() != nullptr) {
            _textureId = GLTextureManager::sharedInstance().getTextureID(image);
            if (image->pixelFormat() == XResource::XImagePixelFormat::Gray) {
                mIsAlphaTexture = true;  //For draw text with color now, I had better use argument to replace this logic
            }
        }
        
        GLfloat glColor[4] = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };
        size_t size = sizeof(glColor);
        for (int i = 0; i < 4; ++i) {
            memcpy(&_color[i * 4], glColor, size);
        }
        buildVAO();
    }
    
    int GLRenderSquareData::getStencilReadWriteValue() {
        const IXRenderDataPovider &p = _belongRender->getRenderDataPovider();
        int a = (p.rd_ClipsLayerIndex() << 4) + (p.rd_BeClipsDrawLayerIndex(mIsClips) & 0x0F);
        return a;
    }
    
    void GLRenderSquareData::render() {
        GLRenderSquareData::sProgram.enable();
        
        if(mIsClips) {
            glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
            //glStencilMask(0xFF);
            glStencilFunc(GL_GEQUAL, _belongRender->getRenderDataPovider().rd_BeClipsDrawLayerIndex(mIsClips), 0xFF);
        } else {
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            //glStencilMask(0x00);
            glStencilFunc(GL_GEQUAL, _belongRender->getRenderDataPovider().rd_BeClipsDrawLayerIndex(mIsClips), 0xFF);
        }
        
        
        sProgram.setUniformMatrix4fv("viewMat", 1, _belongRender->getRenderDataPovider().rd_Transform()._transformMat);
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
    
}