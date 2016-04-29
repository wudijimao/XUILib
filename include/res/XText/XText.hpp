#pragma once
#include "../../core/stdafx.hpp"
#include "../XString.hpp"
#include "XFont.hpp"

namespace XResource {
    
    class XCoreTextChar {
    public:
        XRect mRect;
        wchar_t mUnicodeChar;
    };
    
    class XCoreTextGroup {
    public:
        std::vector<XCoreTextChar*> mChars;
    };
    
    class XCoreTextLine {
    public:
        std::vector<XCoreTextGroup*> mGroups;
    };
    
    class XCoreTextFrame {
    public:
        std::vector<XCoreTextLine*> mLines;
    };
    
    
	class XAttributeString {
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
        std::shared_ptr<XCoreTextFrame> createFrame(){
            auto frame = std::make_shared<XCoreTextFrame>();
            auto line = new XCoreTextLine();
            frame->mLines.push_back(line);
            auto group = new XCoreTextGroup();
            line->mGroups.push_back(group);
            double x = 0;
            for (auto c : mUnicodeCacheStr) {
                auto textChar = new XCoreTextChar();
                textChar->mUnicodeChar = c;
                textChar->mRect.X(x);
                textChar->mRect.Y(10.0);
                textChar->mRect.Width(20.0);
                textChar->mRect.Height(20.0);
                group->mChars.push_back(textChar);
                x += 30.0;
            }
            return frame;
        }
	private:
		XString mStr;
		std::wstring mUnicodeCacheStr;
	};
}



