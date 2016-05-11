#include "XFont.hpp"
#include "XFreeType.hpp"

namespace XResource
{
	    
    
    std::shared_ptr<XFont> XFont::font(const char *fontName, float size) {
        std::shared_ptr<XFont> font;
        font.reset(new XFont());
        font->mFontSize = size;
        font->mFace = XFreeType::sharedInstance()->getMostFitFaceForName(fontName);
        font->mFontName = font->mFace->name();
        return font;
    }
    std::shared_ptr<XFont> XFont::systemFont(float size) {
        std::shared_ptr<XFont> font;
        font.reset(new XFont());
        font->mFontSize = size;
        font->mFace = XFreeType::sharedInstance()->getSystemFace();
        font->mFontName = font->mFace->name();
        return font;
    }
    
    
}














