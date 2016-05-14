//
//  XFreeType.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/11.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include <math.h>
#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
#include "XCoreTextFrame.hpp"
#include "../XImage/XImage.hpp"
#include <assert.h>
#include "XFont.hpp"
#include "../../core/XResManager.hpp"

namespace XResource {
    class XFreeTypeFace {
    public:
        ~XFreeTypeFace() {
            for (auto pair : mGlyphCaches) {
                delete pair.second;
            }
        }
        FT_Face mFace;
        const char *name() {
            return mFace->family_name;
        }
        bool setSize(float in_size) {
            unsigned int size = in_size * XResource::gHighResolutionPixelScale;
            if(size == mSize) {
                return true;
            }
            mSize = size;
            bool ret = true;
            if (mFace->num_fixed_sizes > 0) {
                FT_Error error = FT_Select_Size(mFace, 0);
                
            }
            else {
                FT_Error error = FT_Set_Pixel_Sizes(mFace,   // handle to face object
                                                    0,      // pixel_width
                                                    size);   // pixel_height
                if (error) {
                    ret = false;
                }
                //        error = FT_Set_Char_Size(
                //                                 mFace,    // handle to face object
                //                                 0,       // char_width in 1/64th of points
                //                                 64*16,   // char_height in 1/64th of points
                //                                 300,     // horizontal device resolution
                //                                 300 );   // vertical device resolution
                //        if (error) {
                //            ret = false;
                //        }
            }
            makeFontMetrics();
            selectSizeGlyphMetricsMap();
            return ret;
        }
        inline bool loadGlyph(FT_UInt glyph_index) {
            if (glyph_index > 0) {
                int error = FT_Load_Glyph(mFace, glyph_index, FT_LOAD_COLOR);
                if (error) {
                    return false;
                }
                //FT_Glyph *glyph;
                //FT_Get_Glyph(face->glyph,&glyph);
                return true;
            }
            return false;
        }
        FT_GlyphSlot getGlyph(wchar_t utf8CharctorCode) {
            FT_UInt glyph_index = FT_Get_Char_Index(mFace, utf8CharctorCode);
            if (this->loadGlyph(glyph_index)) {
                return mFace->glyph;
            }
            else {
                return nullptr; //TODO loadFromOtherFont
            }
        }
        XGlyphPtr getXGlyph(wchar_t utf16CharctorCode) {
            auto iter = mGlyphCacheAtCurrentSize->find(utf16CharctorCode);
            if(iter != mGlyphCacheAtCurrentSize->end()) {
                return iter->second;
            }
            XGlyphPtr gly = std::make_shared<XGlyph>();
            std::shared_ptr<IXImage> image;
            FT_GlyphSlot glyph = this->getGlyph(utf16CharctorCode);
            if (glyph != nullptr) {
                if (glyph->format != FT_GLYPH_FORMAT_BITMAP) {
                    int error = FT_Render_Glyph(glyph,   // glyph slot
                                                FT_RENDER_MODE_NORMAL); // render mode
                    if (error) {
                        return nullptr;
                    }
                }
                const FT_Bitmap *bitMap = &(glyph->bitmap);
                //memcpy(&gly->mMetrics, &glyph->metrics, sizeof(FT_Glyph_Metrics));
                double temp = 64.0 * XResource::gHighResolutionPixelScale;
                gly->mMetrics.height = glyph->metrics.height / temp;
                gly->mMetrics.horiAdvance = glyph->metrics.horiAdvance / temp;
                gly->mMetrics.horiBearingX = glyph->metrics.horiBearingX / temp;
                gly->mMetrics.horiBearingY = glyph->metrics.horiBearingY / temp;
                gly->mMetrics.vertAdvance = glyph->metrics.vertAdvance / temp;
                gly->mMetrics.vertBearingX = glyph->metrics.vertBearingX / temp;
                gly->mMetrics.vertBearingY = glyph->metrics.vertBearingY / temp;
                gly->mMetrics.width = glyph->metrics.width / temp;
                gly->mFontMetrics = mFontMetrics;
                if (bitMap->pixel_mode == FT_PIXEL_MODE_GRAY) {
                    auto data = XData::data();
                    data->appendData(bitMap->buffer, bitMap->width * bitMap->rows);
                    image.reset(new XPixelImage(data, bitMap->width, bitMap->rows, XImagePixelFormat::Gray));
                    gly->mImage = image;
                    gly->mImageLeft = glyph->bitmap_left / XResource::gHighResolutionPixelScale;
                    gly->mImageTop = glyph->bitmap_top / XResource::gHighResolutionPixelScale;
                    //image = XImage::imageNamed("a0619F6F8.png");
                    //CGDataProvider *provider = CGDataProviderCreateWithData(NULL, bitMap->buffer, bitMap->width * bitMap->rows, NULL);
                    //CGImage *cgImage = CGImageCreate(bitMap->width, bitMap->rows, 8, 8, bitMap->width, CGColorSpaceCreateDeviceGray(), kCGBitmapByteOrderDefault, provider, NULL, NO, kCGRenderingIntentDefault);
                    //UIImage *image = [UIImage imageWithCGImage : cgImage];
                }
            }
            (*mGlyphCacheAtCurrentSize)[utf16CharctorCode] = gly;
            return gly;
        }
    private:
        void makeFontMetrics() {
            mFontMetrics = std::make_shared<XFontMetrics>();
            FT_Size size = mFace->size;
            double temp = 64.0 * XResource::gHighResolutionPixelScale;
            mFontMetrics->ascender = size->metrics.ascender / temp;
            mFontMetrics->descender = size->metrics.descender / temp;
            mFontMetrics->height = size->metrics.height / temp;
            mFontMetrics->max_advance = size->metrics.max_advance / temp;
        }
        void selectSizeGlyphMetricsMap() {
            auto iter = mGlyphCaches.find(mSize);
            if (iter != mGlyphCaches.end()) {
                mGlyphCacheAtCurrentSize = iter->second;
            } else {
                mGlyphCacheAtCurrentSize = new std::map<wchar_t, XGlyphPtr>();
                mGlyphCaches[mSize] = mGlyphCacheAtCurrentSize;
            }
        }
        unsigned int mSize = 0;
        std::map<wchar_t, XGlyphPtr>* mGlyphCacheAtCurrentSize = nullptr;
        std::shared_ptr<XFontMetrics> mFontMetrics;
        std::map<unsigned int, std::map<wchar_t, XGlyphPtr>*> mGlyphCaches;
    };
    
    class XFreeType {
    public:
        static std::shared_ptr<XFreeType> sharedInstance() {
            static std::shared_ptr<XFreeType> instance = nullptr;
            if (!instance) {
                instance = std::make_shared<XFreeType>();
                instance->Init();
            }
            return instance;
        }
        const std::shared_ptr<XFreeTypeFace>& getMostFitFaceForName(const char *name) {
            auto iter = _faceMap.find(name);
            if (iter != _faceMap.end()) {
                return iter->second;
            } else {
                return getSystemFace();
            }
        }
        const std::shared_ptr<XFreeTypeFace>& getSystemFace() {
            return _faces.front();
        }
        std::shared_ptr<XFreeTypeFace> getFace(const char *path) {
            auto iter = _facePathMap.find(path);
            if (iter != _facePathMap.end()) {
                return iter->second;
            }
            return loadFace(path);
        }
        ~XFreeType() {
            //			for (auto glyhp : _glyphCache) {
            //				FT_Done_Glyph(glyhp);
            //			}
        }
        XGlyphPtr* getXGlyphs(const XFont *font, const wchar_t *utf16Char, int count) {
            XGlyphPtr *glyphs = new XGlyphPtr[count];
            wchar_t *charPtr = (wchar_t*)utf16Char;
            auto facePtr = font->mFace.get();
            facePtr->setSize(font->mFontSize);
            for (int i = 0; i < count; ++i, ++charPtr) {
                auto glyph = facePtr->getXGlyph(*charPtr);
                if (glyph == nullptr) {
                    for (auto face : _faces) {
                        if (face.get() != facePtr) {
                            glyph = face->getXGlyph(*charPtr);
                            if (glyph != nullptr) {
                                break;
                            }
                        }
                    }
                }
                glyphs[i] = glyph;
            }
            return glyphs;
        }
    private:
        FT_Library  _library = nullptr;   // handle to library
        std::vector<std::shared_ptr<XFreeTypeFace>> _faces;
        std::map<std::string, std::shared_ptr<XFreeTypeFace>> _faceMap;
        std::map<std::string, std::shared_ptr<XFreeTypeFace>> _facePathMap;
        // std::vector<FT_Glyph> _glyphCache;
        bool Init() {
            FT_Error error = FT_Init_FreeType(&_library);
            if (error)
            {
                return false;
            }
            FT_Int major, minor, patch;
            FT_Library_Version(_library, &major, &minor, &patch);
            loadDefaultFaces();
            return true;
        }
        void loadDefaultFaces() {
            std::vector<std::string> fontPaths;
            fontPaths.push_back("/System/Library/Fonts/PingFang.ttc");
            fontPaths.push_back("/Users/ximiao/Downloads/Signed_NotoColorEmoji/system/fonts/NotoColorEmoji.ttf");
            fontPaths.push_back("C:\\Windows\\Fonts\\msyh.ttf");
            auto path = XBundle::libBundle()->pathForResource("font/FZMWFont", "ttf");
            fontPaths.push_back(path.UTF8CStr());
            for (auto path : fontPaths) {
                this->loadFace(path.c_str());
            }
            assert(_faces.size() != 0);
        }
        std::shared_ptr<XFreeTypeFace> loadFace(const char *path) {
            auto face = std::make_shared<XFreeTypeFace>();
            FT_Error error = FT_New_Face(_library, path, 0, &face->mFace);
            if (error == FT_Err_Unknown_File_Format)
            {
                return nullptr;
            }
            else if (error)
            {
                return nullptr;
            }
            _faces.push_back(face);
            _faceMap[face->mFace->family_name] = face;
            _facePathMap[path] = face;
            return face;
        }
    };

}
