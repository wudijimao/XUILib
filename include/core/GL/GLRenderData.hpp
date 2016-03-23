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
    
    enum class XGLRenderDataType {
        UnKnown,
        Square,
    };
    class XGLRenderData {
        XGLRenderDataType Type() {
            return XGLRenderDataType::UnKnown;
        }
    };
    class XGLRenderSquareData {
        
    };
}