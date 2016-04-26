#pragma once
#include "../../core/stdafx.hpp"
#include "../XString.hpp"
#include "../XResource.hpp"

namespace XResource
{
	class XFont {
	public:
		//systomFonts
		static std::shared_ptr<XFont> fontWithName(const char *fontName);
		float fontSize();
		void setFontSize(float fontSize);
	private:
		static const float sDefaultFontSize;
		float mFontSize;
		XString mFontName;
	protected:
		XFont() {};
		bool initWithFilePath(const char *filePath);
	};
}