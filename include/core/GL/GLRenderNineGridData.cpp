//
//  GLRenderNineGridData.cpp
//  XDUILib
//
//  Created by ximiao on 16/6/24.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "GLRenderNineGridData.hpp"
#include "../GLRender.hpp"

namespace XDUILib {
    GLProgram GLRenderNineGridData::sProgram;
    
    GLRenderDataType GLRenderNineGridData::Type() {
        return GLRenderDataType::NineGrid;
    }

    GLRenderNineGridData::GLRenderNineGridData(GLRender *render) : GLRenderData(render) { }
    
    void GLRenderNineGridData::initWithRect(const XResource::XRect &rect, const std::shared_ptr<XResource::XStretchableImage> &image) {
        memset(_square, 0, sizeof(_square));
        //setPosition(rect.point());
        //_transform = _transform * _belongRender->getRenderDataPovider().rd_Transform();
//        for (auto square : _square) {
//            square.z = 0.5f;
//        }
        double imageWidth = image->image()->size().Width();
        double imageHeight = image->image()->size().Height();
        GLfloat y = rect.Y();
        GLfloat v = 0;
        int i = 0;
        while (i < 4) {
            _texturePos[i].v = v;
            _square[i++].y = y;
        }
        y += image->stretchEdge().top();
        v = image->stretchEdge().top() / imageHeight;
        while (i < 8) {
            _texturePos[i].v = v;
            _square[i++].y = y;
        }
        y = rect.bY() - image->stretchEdge().bottom();
        v = (imageHeight - image->stretchEdge().bottom()) / imageHeight;
        while (i < 12) {
            _texturePos[i].v = v;
            _square[i++].y = y;
        }
        y = rect.bY();
        v = 1.0f;
        while (i < 16) {
            _texturePos[i].v = v;
            _square[i++].y = y;
        }
        
        
        GLfloat x = rect.X();
        GLfloat u = 0;
        i = 0;
        while (i < 16) {
            _texturePos[i].u = u;
            _square[i].x = x;
            i += 4;
        }
        x += image->stretchEdge().left();
        u = image->stretchEdge().left() / imageWidth;
        i = 1;
        while (i < 16) {
            _texturePos[i].u = u;
            _square[i].x = x;
            i += 4;
        }
        x = rect.rX() - image->stretchEdge().right();
        u = (imageWidth - image->stretchEdge().right()) / imageWidth;
        i = 2;
        while (i < 16) {
            _texturePos[i].u = u;
            _square[i].x = x;
            i += 4;
        }
        x = rect.rX();
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
        sProgram.setUniformMatrix4fv("viewMat", 1, _belongRender->getRenderDataPovider().rd_Transform()._transformMat);
        sProgram.setUniformValue("uIsClipsToBounds", false);
        
 
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        //glStencilMask(0x00);
        glStencilFunc(GL_GEQUAL, _belongRender->getRenderDataPovider().rd_BeClipsDrawLayerIndex(false), 0xFF);
        
        if (_textureId > 0) {
            glActiveTexture(GL_ACTIVE_TEXTURE - 1);
            glBindTexture(GL_TEXTURE_2D, _textureId);
            GLRenderSquareData::sProgram.setUniformValue("s_texture", GL_ACTIVE_TEXTURE - GL_TEXTURE0 - 1);
            
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