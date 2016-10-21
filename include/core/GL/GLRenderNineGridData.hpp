//
//  GLRenderNineGridData.hpp
//  XDUILib
//
//  Created by ximiao on 16/6/24.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma once

#include "GLRenderData.hpp"

namespace XDUILib {
    class GLRenderNineGridData : public GLRenderData {
    public:
        GLRenderNineGridData(GLRender *render);
        virtual GLRenderDataType Type() override;
        static GLProgram sProgram;
        GLPos3D _square[16];
        GLTexturePos2D _texturePos[16];
        GLubyte indices[54]; //total nine square. And two triangle per square.
        GLuint  _textureId = 0;
        void initWithRect(const XResource::XRect &rect, const std::shared_ptr<XResource::XStretchableImage> &image);
        virtual void render() override;
        GLuint bufObjects[3];
        void buildVAO();
    };
}