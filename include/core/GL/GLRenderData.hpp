//
//  GLRenderData.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

//TODO::not Use Rect pos as  3D pos,  but as a transformational matrix.  can improve move prefromance?

#pragma once

#include "GLTexture.hpp"
#include "GLProgram.hpp"
#include "GLTransform3D.hpp"
#include <sstream>

class GLRender;

namespace XDUILib {
    
    enum class GLRenderDataType {
        UnKnown,
        Square,
        NineGrid,
    };
    
    struct GLPos3D {
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };
    struct GLTexturePos2D {
        GLfloat u;
        GLfloat v;
    };
    
    class GLRenderData {
    public:
        virtual ~GLRenderData();
        GLRenderData(GLRender *render);
        GLuint _vectexArrayObject;
        virtual GLRenderDataType Type();
        virtual void render() = 0;
        virtual void setPosition(const XResource::XDisplayPoint &point) final;
        virtual void move(const XResource::XDisplayPoint &point) final;
    protected:
        GLTransform3D _transform;
        const GLRender *_belongRender;
    };
    class GLRenderSquareData : public GLRenderData {
    public:
        GLRenderSquareData(GLRender *render);
        GLfloat _clipsX1, _clipsX2, _clipsY1, _clipsY2;
        bool mIsClips = false;
        
		static GLProgram sProgram;
        GLfloat _square[12];
        //GLubyte _indices[6];
        GLfloat _texturePos[8];
        GLfloat _color[16];
        GLuint  _textureId = 0;
        GLuint  _maskTextureId = 0;
        
        virtual GLRenderDataType Type();
        void setClips(bool clips);
        void setClipsBound(const XResource::XRect &rect);
        void setMaskImage(const std::shared_ptr<XResource::IXImage> image);
        
        void setSquare(const XResource::XRect &rect);
        void initWithRect(const XResource::XRect &rect, const XResource::XColor &color, const std::shared_ptr<XResource::IXImage> &image);
        virtual void render() override;
    private:
        bool mIsAlphaTexture;
        virtual ~GLRenderSquareData();
        GLuint bufObjects[3];//[4];
        void buildVAO();
    };
}




