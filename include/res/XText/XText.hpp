#pragma once
#include "../../core/stdafx.hpp"
#include "../XString.hpp"
#include "XFont.hpp"

namespace XResource {
    
    
    struct XRange {
        XRange(){}
        XRange(unsigned long in_location, unsigned long in_length):location(in_location), length(in_length){}
        inline unsigned long rightPosition() const {
            return location + length;
        }
        unsigned long location = ULONG_MAX;
        unsigned long length = 0;
    };
    
    typedef std::shared_ptr<XStringAttr> XStrAttrPtr;
    
    struct XStrAttrContainer {
        XStrAttrPtr attr;
        XRange range;
        XStrAttrContainer(){}
        XStrAttrContainer(const XStrAttrPtr &in_attr, const XRange &in_range) : range(in_range), attr(in_attr) {}
    };
    
    typedef std::vector<XStrAttrContainer> XStrAttrVec;
    
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
		std::shared_ptr<XCoreTextFrame> createFrame(const XResource::XRect &xRect) const;
        void clearAttrs();
        void clearAttrs(XAttrStrKeyEnum type);
        void addAttr(const XStrAttrPtr &attr);
        void addAttr(const XStrAttrPtr &attr, const XRange &range);
        void addAttrs(const XStrAttrVec &attr, unsigned long location, unsigned long length);
        XStrAttrVec getAttrs(unsigned long in_loc, XRange &out_effactRange) const;
        const XStrAttrPtr& getAttr(unsigned long in_loc, XAttrStrKeyEnum type, XRange &out_effactRange) const;
	private:
        void fillTextColor(XCoreTextFrame &frame) const;
		XString mStr;
		std::wstring mUnicodeCacheStr;
        std::map<XAttrStrKeyEnum, XStrAttrVec> mTypedAttrs;
        XStrAttrPtr mEmptyAttr;
	};
    
   
    
}



