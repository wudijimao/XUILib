#include "XFont.hpp"
#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
#include "../XImage/XImage.hpp"
#include "XText.hpp"

namespace XResource
{
	class XFreeTypeFace {
	public:
		FT_Face mFace;
		bool setSize(unsigned int size) {
            size *= XResource::gHighResolutionPixelScale;
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
		XGlyphPtr getXGlyph(wchar_t utf8CharctorCode) {
            XGlyphPtr gly = std::make_shared<XGlyph>();
			std::shared_ptr<IXImage> image;
			FT_GlyphSlot glyph = this->getGlyph(utf8CharctorCode);
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
                gly->mMetrics.reset(new XGlyphMetrics());
                gly->mMetrics->height = glyph->metrics.height / temp;
                gly->mMetrics->horiAdvance = glyph->metrics.horiAdvance / temp;
                gly->mMetrics->horiBearingX = glyph->metrics.horiBearingX / temp;
                gly->mMetrics->horiBearingY = glyph->metrics.horiBearingY / temp;
                gly->mMetrics->vertAdvance = glyph->metrics.vertAdvance / temp;
                gly->mMetrics->vertBearingX = glyph->metrics.vertBearingX / temp;
                gly->mMetrics->vertBearingY = glyph->metrics.vertBearingY / temp;
                gly->mMetrics->width = glyph->metrics.width / temp;
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
			return gly;
		}
    private:
        void makeFontMetrics() {
            if(mFontMetrics.get() == nullptr) {
                mFontMetrics.reset(new XFontMetrics());
            }
            FT_Size size = mFace->size;
            double temp = 64.0 * XResource::gHighResolutionPixelScale;
            mFontMetrics->ascender = size->metrics.ascender / temp;
            mFontMetrics->descender = size->metrics.descender / temp;
            mFontMetrics->height = size->metrics.height / temp;
            mFontMetrics->max_advance = size->metrics.max_advance / temp;
        }
        std::shared_ptr<XFontMetrics> mFontMetrics;
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
		std::shared_ptr<XFreeTypeFace> getFace(const char *path) {
			auto iter = _facePathMap.find(path);
			if (iter != _facePathMap.end()) {
				return iter->second;
			}

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

		//		FT_GlyphSlot getGlyph(int32_t utf8CharctorCode, const char *fontName) {
		//			FT_UInt glyph_index = 0;
		//			FT_Face face = nullptr;
		//			do {
		//				auto iter = _faceMap.find(fontName);
		//				if (iter != _faceMap.end()) {
		//					face = iter->second;
		//					glyph_index = FT_Get_Char_Index(face, utf8CharctorCode);
		//					if (this->loadGlyph(face, glyph_index)) {
		//						return face->glyph;
		//					}
		//				}
		//				for (auto f : _faces) {
		//					if (f != face) {
		//						glyph_index = FT_Get_Char_Index(f, utf8CharctorCode);
		//						if (glyph_index > 0) {
		//							if (this->loadGlyph(f, glyph_index)) {
		//								return f->glyph;
		//							}
		//						}
		//					}
		//				}
		//			} while (0);
		//			return nullptr;
		//		}


		FT_Library  _library = nullptr;   // handle to library
		std::vector<std::shared_ptr<XFreeTypeFace>> _faces;
		std::map<std::string, std::shared_ptr<XFreeTypeFace>> _faceMap;
		std::map<std::string, std::shared_ptr<XFreeTypeFace>> _facePathMap;
		std::vector<FT_Glyph> _glyphCache;
		~XFreeType() {
			for (auto glyhp : _glyphCache) {
				FT_Done_Glyph(glyhp);
			}
		}
		bool Init() {
			FT_Error error = FT_Init_FreeType(&_library);
			if (error)
			{
				return false;
			}
			FT_Int major, minor, patch;
			FT_Library_Version(_library, &major, &minor, &patch);
			return true;
		}
		void loadDefaultFaces() {
			this->getFace("/Users/ximiao/Downloads/Signed_NotoColorEmoji/system/fonts/NotoColorEmoji.ttf");
			//this->loadFace("/System/Library/Fonts/Apple Color Emoji.ttf");
			this->getFace("/System/Library/Fonts/PingFang.ttc");
		}
	private:
	};
    int testToken = 0;
	std::shared_ptr<XCoreTextFrame> XAttributeString::createFrame(const XResource::XRect &xRect) {
		static std::shared_ptr<XCoreTextFrame> frame;
		if (testToken == 0) {
			++testToken;
			frame = std::make_shared<XCoreTextFrame>();
			auto line = new XCoreTextLine();
			frame->mLines.push_back(line);
			auto group = new XCoreTextGroup();
			line->mGroups.push_back(group);
			double x = xRect.X();
            double y = xRect.Y();
			//auto face = XFreeType::sharedInstance()->getFace("C:\\Windows\\Fonts\\msyh.ttf");
            auto face = XFreeType::sharedInstance()->getFace("/System/Library/Fonts/PingFang.ttc");
			face->setSize(40);
			for (auto c : mUnicodeCacheStr) {
				auto textChar = new XCoreTextChar();
                if (face) {
                    textChar->mGlyph = face->getXGlyph(c);
                }
//                textChar->mRect.X(x);
//                textChar->mRect.Y(y);
//                x += 50;
//                textChar->mRect.Width(30);
//                textChar->mRect.Height(30);
                
				textChar->mRect.X(x + textChar->mGlyph->mImageLeft);
				textChar->mRect.Y(y + textChar->mGlyph->mFontMetrics->ascender  - textChar->mGlyph->mImageTop);
				textChar->mRect.setSize(textChar->mGlyph->mImage->size());
				group->mChars.push_back(textChar);
                x += textChar->mGlyph->mMetrics->horiAdvance;
			}
		}
		return frame;
	}
}


