#pragma once
#include "../../core/stdafx.hpp"
#include "../XString.hpp"
#include "../XRect.hpp"
#include "../XImage/XImage.hpp"
#include "../XColor.hpp"
#include "XStringAttr.hpp"

namespace XResource
{
    
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
        
}