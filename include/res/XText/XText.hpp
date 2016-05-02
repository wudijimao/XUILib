#pragma once
#include "../../core/stdafx.hpp"
#include "../XString.hpp"
#include "XFont.hpp"

namespace XResource {
    
    class SIMPLEDIRECTUI_API XCoreTextChar {
    public:
        XRect mRect;
        wchar_t mUnicodeChar;
		std::shared_ptr<IXImage> mImage;
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
		std::shared_ptr<XCoreTextFrame> createFrame();
	private:
		XString mStr;
		std::wstring mUnicodeCacheStr;
	};
}



