#include "XFont.hpp"
#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H

namespace XResource
{
	class TextGlyph {

	};

	class XFont {
	public:
		static std::shared_ptr<XFont> fontWithName();
	private:
		FT_Face mFace;
	};

	class XFreeType {
	public:
		bool init(const char *path) {
			FT_Error error = FT_New_Face(_library, path, 0, &mFace);
			if (error == FT_Err_Unknown_File_Format)
			{
				return false;
			}
			else if (error)
			{
				return false;
			}
			this->setSize(face, 40);
			return true;
		}
		bool setSize(FT_Face face, unsigned int size) {
			bool ret = true;
			if (face->num_fixed_sizes > 0) {
				FT_Error error = FT_Select_Size(face, 0);

			}
			else {
				FT_Error error = FT_Set_Pixel_Sizes(face,   // handle to face object 
					0,      // pixel_width          
					size);   // pixel_height          
				if (error) {
					ret = false;
				}
				//        error = FT_Set_Char_Size(
				//                                 face,    // handle to face object
				//                                 0,       // char_width in 1/64th of points
				//                                 64*16,   // char_height in 1/64th of points
				//                                 300,     // horizontal device resolution
				//                                 300 );   // vertical device resolution
				//        if (error) {
				//            ret = false;
				//        }
			}
			return ret;
		}
		FT_GlyphSlot getGlyph(int32_t utf8CharctorCode, const char *fontName) {
			FT_UInt glyph_index = 0;
			FT_Face face = nullptr;
			do {
				auto iter = _faceMap.find(fontName);
				if (iter != _faceMap.end()) {
					face = iter->second;
					glyph_index = FT_Get_Char_Index(face, utf8CharctorCode);
					if (this->loadGlyph(face, glyph_index)) {
						return face->glyph;
					}
				}
				for (auto f : _faces) {
					if (f != face) {
						glyph_index = FT_Get_Char_Index(f, utf8CharctorCode);
						if (glyph_index > 0) {
							if (this->loadGlyph(f, glyph_index)) {
								return f->glyph;
							}
						}
					}
				}
			} while (0);
			return nullptr;
		}

		inline BOOL loadGlyph(FT_Face face, FT_UInt glyph_index) {
			if (glyph_index > 0) {
				int error = FT_Load_Glyph(face, glyph_index, FT_LOAD_COLOR);
				if (error) {
					return false;
				}
				//FT_Glyph *glyph;
				//FT_Get_Glyph(face->glyph,&glyph);
				return true;
			}
			return false;
		}
		FT_Library  _library;   // handle to library
		std::vector<FT_Face> _faces;
		std::map<std::string, FT_Face> _faceMap;
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
		bool loadFace(const char *path) {
			FT_Face face;
			FT_Error error = FT_New_Face(_library,
				path,//,///
				0,
				&face);
			_faces.push_back(face);
			_faceMap[face->family_name] = face;
			if (error == FT_Err_Unknown_File_Format)
			{
				return false;
			}
			else if (error)
			{
				return false;
			}
			this->setSize(face, 40);
			return true;
		}
		void loadDefaultFaces() {
			this->loadFace("/Users/ximiao/Downloads/Signed_NotoColorEmoji/system/fonts/NotoColorEmoji.ttf");
			//this->loadFace("/System/Library/Fonts/Apple Color Emoji.ttf");
			this->loadFace("/System/Library/Fonts/PingFang.ttc");
		}
		UIImage *drawText(const XText &str) {
			unsigned int i = 0, length = str.length()
				wchar_t *utf16char;
			UIGraphicsBeginImageContext(CGSizeMake(200, 200));
			float x = 0.0f;
			while (i < length) {
				U8_NEXT(string.getUTF8String()->c_str(), i, length, textIndex);
				FT_GlyphSlot glyph = this->getGlyph(textIndex, "Apple Color Emoji");
				if (glyph != nullptr) {
					if (glyph->format != FT_GLYPH_FORMAT_BITMAP) {
						int error = FT_Render_Glyph(glyph,   // glyph slot
							FT_RENDER_MODE_NORMAL); // render mode
						if (error) {
							return nullptr;
						}
					}
					const FT_Bitmap *bitMap = &(glyph->bitmap);
					if (bitMap->pixel_mode == FT_PIXEL_MODE_GRAY) {
						CGDataProvider *provider = CGDataProviderCreateWithData(NULL, bitMap->buffer, bitMap->width * bitMap->rows, NULL);
						CGImage *cgImage = CGImageCreate(bitMap->width, bitMap->rows, 8, 8, bitMap->width, CGColorSpaceCreateDeviceGray(), kCGBitmapByteOrderDefault, provider, NULL, NO, kCGRenderingIntentDefault);
						UIImage *image = [UIImage imageWithCGImage : cgImage];
						CGImageRelease(cgImage);
						CGDataProviderRelease(provider);
						[image drawInRect : CGRectMake(x, 0, 20, 20)];
					}
					x += 22.0f;
				}
			}
			UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
			UIGraphicsEndImageContext();
			return image;
		}
	private:
	};
}
