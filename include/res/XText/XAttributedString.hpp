#pragma once
#include "../../core/stdafx.hpp"
#include "../XString.hpp"
#include "XFont.hpp"
#include "XStringAttr.hpp"
#include "XStringAttrColor.hpp"
#include "XCoreTextFrame.hpp"

namespace XResource {
    
    class XAttributedString;
    typedef std::shared_ptr<XAttributedString> XAttributedStringPtr;
    
	class SIMPLEDIRECTUI_API XAttributedString {
    public:
        static XAttributedStringPtr attrStr(const XString &str) {
            return std::make_shared<XAttributedString>(str);
        }
		XAttributedString(const XString &str) {
			mStr = str;
            mUnicodeCacheStr = mStr.getUnicodeString();
		}
		unsigned long length() {
			return mUnicodeCacheStr.length();
		}
		const wchar_t *getUinicodeCStr() {
			return mUnicodeCacheStr.c_str();
		}
		std::shared_ptr<XCoreTextFrame> createFrame(const XResource::XDisplaySize &mSize) const;
        void clearAttrs();
        void clearAttrs(XAttrStrKeyEnum type);
        void addAttr(const XStrAttrPtr &attr);
        void addAttr(const XStrAttrPtr &attr, const XRange &range);
        void addAttrs(const XStrAttrVec &attr, unsigned long location, unsigned long length);
        XStrAttrVec getAttrs(unsigned long in_loc, XRange &out_effactRange) const;
        const XStrAttrPtr& getAttr(unsigned long in_loc, XAttrStrKeyEnum type, XRange &out_effactRange) const;
	private:
        void clearFrameCache();
        std::shared_ptr<XCoreTextFrame> mFrameCache;
        void fillTextColor(XCoreTextFrame &frame) const;
		XString mStr;
		std::wstring mUnicodeCacheStr;
        std::map<XAttrStrKeyEnum, XStrAttrVec> mTypedAttrs;
        XStrAttrPtr mEmptyAttr;
	};
    
}



