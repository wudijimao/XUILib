#pragma once
#include "../../core/stdafx.hpp"
#include "../XString.hpp"
#include "XFont.hpp"

namespace XResource {
    
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



