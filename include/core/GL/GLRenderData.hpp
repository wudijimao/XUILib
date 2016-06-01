//
//  GLRenderData.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

//TODO::not Use Rect pos as  3D pos,  but as a transformational matrix.  can improve move prefromance?

#pragma once

#include "../stdafx.hpp"
#include "../../res/XResource.hpp"
#include "GLHeaders.h"
#include "GLProgram.hpp"
#include "GLTransform3D.hpp"
#include <sstream>

namespace XDUILib {
	class GLTexture {
	public:
		std::shared_ptr<XResource::XImage> Img() {
			return std::shared_ptr<XResource::XImage>(_img);
		}
		GLuint glTextureIndex() {
			return _glTextureIndex;
		}
	protected:
		GLTexture(std::shared_ptr<XResource::XImage> img, int glTextureIndex) {
			_img = img;
			_glTextureIndex = glTextureIndex;
		}
	private:
		std::weak_ptr<XResource::XImage> _img;
		GLuint _glTextureIndex;
	};
	class GLTextureManager {
		std::map<XResource::IXImage *, GLuint> textureMap;
	public:
		//临时  应该一个Canvas对应一个manager？
		static GLTextureManager &sharedInstance() {
			static GLTextureManager manager;
			return manager;
		}

		//TODO::需要支持淘汰机制 和 多图（gif）
		GLuint getTextureID(const std::shared_ptr<XResource::IXImage> &image) {
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
		GLuint loadTexture(XResource::IXImage *image) {
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
			{
				std::stringstream s;
				s << "a" << (void*)image << ".png";
				//image->saveToFile(s.str().c_str());
			}
				format = GL_RGBA;
				break;
			case XResource::XImagePixelFormat::RGB24:
			{
				std::stringstream s;
				s << "a" << (void*)image << ".png";
				//image->saveToFile(s.str().c_str());
			}
				format = GL_RGB;
				break;
			case XResource::XImagePixelFormat::Gray:
			{
				std::stringstream s;
				s << "a" << (void*)image << ".png";
				//image->saveToFile(s.str().c_str());
			}
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
    };
    
    
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
        virtual ~GLRenderData(){};
        GLRenderData() {
        }
        GLuint _vectexArrayObject;
        virtual GLRenderDataType Type() {
            return GLRenderDataType::UnKnown;
        }
        virtual void render() = 0;
        virtual void setPosition(const XResource::XDisplayPoint &point) final {
            _transform.setPosition(point.X(), point.Y());
        }
        virtual void move(const XResource::XDisplayPoint &point) final {
            _transform.move(point.X(), point.Y());
        }
    protected:
        GLTransform3D _transform;
    };
    class GLRenderSquareData : public GLRenderData {
    public:
        GLfloat _clipsX1, _clipsX2, _clipsY1, _clipsY2;
        bool mIsClips = false;
        
		static GLProgram sProgram;
        GLfloat _square[12];
        //GLubyte _indices[6];
        GLfloat _texturePos[8];
        GLfloat _color[16];
        GLuint  _textureId = 0;
        GLuint  _maskTextureId = 0;
        
        virtual GLRenderDataType Type() {
            return GLRenderDataType::Square;
        }
        
        void setClips(bool clips) {
            mIsClips = clips;
        }
        
        void setClipsBound(const XResource::XRect &rect) {
            _clipsX1 = rect.X();
            _clipsX2 = _clipsX1 + rect.Width();
            _clipsY1 = rect.Y();
            _clipsY2 = _clipsY1 + rect.Height();
        }
        
        void setMaskImage(const std::shared_ptr<XResource::IXImage> image) {
            if (image.get() != nullptr) {
                _maskTextureId = GLTextureManager::sharedInstance().getTextureID(image);
            } else {
                _maskTextureId = 0;
            }
        }
        
        void setSquare(const XResource::XRect &rect) {
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
            
            //test
//            _indices[0] = 0;
//            _indices[1] = 1;
//            _indices[2] = 2;
//            _indices[3] = 1;
//            _indices[4] = 2;
//            _indices[5] = 3;
        }
        
        void initWithRect(const XResource::XRect &rect, const XResource::XColor &color, const std::shared_ptr<XResource::IXImage> &image) {
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
       virtual void render() override {
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
    private:
        bool mIsAlphaTexture;
        virtual ~GLRenderSquareData() {
            GLRenderData::~GLRenderData();
            glDeleteBuffers(3, bufObjects);
            glDeleteVertexArrays(1, &_vectexArrayObject);
        }
        GLuint bufObjects[3];//[4];
        void buildVAO() {
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
    };
    
    class GLRenderNineGridData : public GLRenderData {
    public:
        virtual GLRenderDataType Type() override {
            return GLRenderDataType::NineGrid;
        }
        static GLProgram sProgram;
        GLPos3D _square[16];
        GLTexturePos2D _texturePos[16];
        GLubyte indices[54]; //total nine square. And two triangle per square.
        GLuint  _textureId = 0;
        void initWithRect(const XResource::XRect &rect, const std::shared_ptr<XResource::XStretchableImage> &image) {
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
        virtual void render() override {
            sProgram.enable();
            sProgram.setUniformMatrix4fv("viewMat", 1, (GLfloat*)&_transform._transformMat);
            sProgram.setUniformValue("uIsClipsToBounds", false);
            
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
        
        
        
        GLuint bufObjects[3];
        void buildVAO() {
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
    };
}




