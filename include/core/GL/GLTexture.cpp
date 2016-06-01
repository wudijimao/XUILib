//
//  GLTexture.cpp
//  XDUILib
//
//  Created by ximiao on 16/6/1.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "GLTexture.hpp"



namespace XDUILib {
    
    std::shared_ptr<XResource::XImage> GLTexture::Img() {
        return std::shared_ptr<XResource::XImage>(_img);
    }
    GLuint GLTexture::glTextureIndex() {
        return _glTextureIndex;
    }
    GLTexture::GLTexture(std::shared_ptr<XResource::XImage> img, int glTextureIndex) {
        _img = img;
        _glTextureIndex = glTextureIndex;
    }
    
    //临时  应该一个Canvas对应一个manager？
    GLTextureManager& GLTextureManager::sharedInstance() {
        static GLTextureManager manager;
        return manager;
    }
    
    //TODO::需要支持淘汰机制 和 多图（gif）
    GLuint GLTextureManager::getTextureID(const std::shared_ptr<XResource::IXImage> &image) {
        XResource::IXImage *img = image.get();
        auto iter = textureMap.find(img);
        GLuint textureId;
        if (iter == textureMap.end()) {
            textureId = loadTexture(img);
            textureMap[img] = textureId;
        }
        else {
            textureId = iter->second;
        }
        return textureId;
    }
    GLuint GLTextureManager::loadTexture(XResource::IXImage *image) {
        GLuint textureId;
        int width = image->width();
        int height = image->height();
        auto data = image->getPixelsData(0);
        
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        GLuint format = 0;
        switch (image->pixelFormat())
        {
            case XResource::XImagePixelFormat::RGBA32:
                format = GL_RGBA;
                break;
            case XResource::XImagePixelFormat::RGB24:
                format = GL_RGB;
                break;
            case XResource::XImagePixelFormat::Gray:
                format = GL_ALPHA;
                break;
            default:
                return 0;
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data->getBuf());
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        return textureId;
    }
    
}