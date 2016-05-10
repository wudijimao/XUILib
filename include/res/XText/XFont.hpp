#pragma once
#include "../../core/stdafx.hpp"
#include "../XString.hpp"
#include "../XRect.hpp"
#include "../XImage/XImage.hpp"
#include "../XColor.hpp"

namespace XResource
{
    enum XAttrStrKeyEnum {
        XAttrStrKey_Font = 0,
        XAttrStrKey_Color = 1,
        XAttrStrKey_Custom = 2000,
        //canAddToBelow or use XAttrStrKey_Customn + 1(2,3,4...)
    };
    
    class XStringAttr {
    public:
        virtual ~XStringAttr(){};
        virtual XAttrStrKeyEnum getKey() = 0;
        //for debug
        const char *getKeyName() {
            XAttrStrKeyEnum key = getKey();
            switch (key) {
                case XAttrStrKey_Font:
                    return "XAttrStrKey_Font";
                case XAttrStrKey_Color:
                    return "XAttrStrKey_Color";
                default:
                    break;
            }
            return "XAttrStrKey_Custom";
        }
    };
    
    class SIMPLEDIRECTUI_API XStringAttrColor : public XStringAttr {
    public:
        static std::shared_ptr<XStringAttrColor> colorWithColor(const std::shared_ptr<XUIColor> color) {
            auto c = std::make_shared<XStringAttrColor>();
            c->mColor = color;
            return c;
        }
        static std::shared_ptr<XStringAttrColor> colorWithColor(const std::shared_ptr<XUIColor> color, const std::shared_ptr<XUIColor> backColor) {
            auto c = std::make_shared<XStringAttrColor>();
            c->mColor = color;
            c->mBackgroundColor = backColor;
            return c;
        }

        static std::shared_ptr<XStringAttrColor> colorWithBackgroundColor(const std::shared_ptr<XUIColor> color) {
            auto c = std::make_shared<XStringAttrColor>();
            c->mBackgroundColor = color;
            return c;
        }

        std::shared_ptr<XUIColor> mColor;
        std::shared_ptr<XUIColor> mBackgroundColor;
        virtual XAttrStrKeyEnum getKey() override {
            return XAttrStrKey_Color;
        }
    };
    
    class XFreeTypeFace;
    class XFreeType;
    class SIMPLEDIRECTUI_API XFont : public XStringAttr {
	public:
        friend XResource::XFreeTypeFace;
        friend XResource::XFreeType;
		//systomFonts
		static std::shared_ptr<XFont> font(const char *fontName, float size);
        static std::shared_ptr<XFont> systemFont(float size);
		float fontSize();
		void setFontSize(float fontSize);
        virtual XAttrStrKeyEnum getKey() override {
            return XAttrStrKey_Font;
        }
	private:
		static const float sDefaultFontSize;
		float mFontSize;
		XString mFontName;
        std::shared_ptr<XFreeTypeFace> mFace;
	protected:
		XFont() {};
	};
    
    
    struct XFontMetrics
    {
        //        unsigned short  x_ppem;      /* horizontal pixels per EM               */
        //        unsigned short  y_ppem;      /* vertical pixels per EM                 */
        
        //        signed long   x_scale;     /* scaling values used to convert font    */
        //        signed long   y_scale;     /* units to 26.6 fractional pixels        */
        
        double     ascender;    /* ascender in 26.6 frac. pixels          */
        double     descender;   /* descender in 26.6 frac. pixels         */
        double     height;      /* text height in 26.6 frac. pixels       */
        double     max_advance; /* max horizontal advance, in 26.6 pixels */
    };
    
    struct XGlyphMetrics {
        double  width;
        double  height;
        
        double  horiBearingX;
        double  horiBearingY;
        double  horiAdvance;
        
        double  vertBearingX;
        double  vertBearingY;
        double  vertAdvance;
    };
    
    //only one for each char(font size...
    class XGlyph {
    public:
        std::shared_ptr<XFontMetrics> mFontMetrics;
        XGlyphMetrics mMetrics;
        std::shared_ptr<IXImage> mImage;
        wchar_t mUnicodeChar;
        signed long mImageLeft;
        signed long mImageTop;
    };
    
    typedef std::shared_ptr<XGlyph> XGlyphPtr;
    
    class SIMPLEDIRECTUI_API XCoreTextChar {
    public:
        XRect mRect;
        XGlyphPtr mGlyph;
        std::shared_ptr<XResource::XUIColor> mFrontColor;
    };
    
    class SIMPLEDIRECTUI_API XCoreTextGroup {
    public:
        ~XCoreTextGroup() {
            for (auto c : mChars) {
                delete c;
            }
        }
        std::vector<XCoreTextChar*> mChars;
    };
    
    class SIMPLEDIRECTUI_API XCoreTextLine {
    public:
        ~XCoreTextLine() {
            for (auto g : mGroups) {
                delete g;
            }
        }
        std::vector<XCoreTextGroup*> mGroups;
    };
    
    class SIMPLEDIRECTUI_API XCoreTextFrame {
    public:
        ~XCoreTextFrame() {
            for (auto line : mLines) {
                delete line;
            }
        }
        std::vector<XCoreTextLine*> mLines;
    };
}