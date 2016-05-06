#pragma once
#include "../../core/stdafx.hpp"
#include "../XString.hpp"
#include "XFont.hpp"

namespace XResource {    
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
        std::shared_ptr<XGlyphMetrics> mMetrics;
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
    };
    
    class SIMPLEDIRECTUI_API XCoreTextGroup {
    public:
        std::vector<XCoreTextChar*> mChars;
    };
    
    class SIMPLEDIRECTUI_API XCoreTextLine {
    public:
        std::vector<XCoreTextGroup*> mGroups;
    };
    
    class SIMPLEDIRECTUI_API XCoreTextFrame {
    public:
        std::vector<XCoreTextLine*> mLines;
    };
    
    
	class SIMPLEDIRECTUI_API XAttributeString {
    public:
		XAttributeString(const XString &str) {
			mStr = str;
            mUnicodeCacheStr = mStr.getUnicodeString();
		}
		unsigned int length() {
			return mUnicodeCacheStr.length();
		}
		const wchar_t *getUinicodeCStr() {
			return mUnicodeCacheStr.c_str();
		}
		std::shared_ptr<XCoreTextFrame> createFrame(const XResource::XRect &xRect);
	private:
		XString mStr;
		std::wstring mUnicodeCacheStr;
	};
}



