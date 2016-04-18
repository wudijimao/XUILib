#pragma once
#include "../../core/stdafx.hpp"
#include "../XString.hpp"
#include "XFont.hpp"

namespace XResource {
	class XText {
		XText(const XString &str) {
			mStr = str;
		}
		unsigned int length() {
			initCache();
			return mUnicodeCacheStr.length();
		}
		const wchar_t *getUinicodeCStr() {
			initCache();
			return mUnicodeCacheStr.c_str();
		}
	private:
		inline void initCache() {
			if (!mIsCached)
			{
				mUnicodeCacheStr = mStr.getUnicodeString();
			}
		}
		XString mStr;
		bool mIsCached = false;
		std::wstring mUnicodeCacheStr;
	};
}