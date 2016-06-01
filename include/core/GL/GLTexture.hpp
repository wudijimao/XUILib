//
//  GLTexture.hpp
//  XDUILib
//
//  Created by ximiao on 16/6/1.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma once

#include "../stdafx.hpp"
#include "../../res/XResource.hpp"
#include "GLHeaders.h"

namespace XDUILib {
    
    
    class GLTexture {
    public:
        std::shared_ptr<XResource::XImage> Img();
        GLuint glTextureIndex();
    protected:
        GLTexture(std::shared_ptr<XResource::XImage> img, int glTextureIndex);
    private:
        std::weak_ptr<XResource::XImage> _img;
        GLuint _glTextureIndex;
    };
    
    
    class GLTextureManager {
        std::map<XResource::IXImage *, GLuint> textureMap;
    public:
        //临时  应该一个Canvas对应一个manager？
        static GLTextureManager &sharedInstance();
        
        //TODO::需要支持淘汰机制 和 多图（gif）
        GLuint getTextureID(const std::shared_ptr<XResource::IXImage> &image);
        
    private:
        GLuint loadTexture(XResource::IXImage *image);
    };
    
}

