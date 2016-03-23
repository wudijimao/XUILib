#pragma once
#include "../core/stdafx.hpp"
#include "XRect.hpp"
#include "XRectPro.hpp"
#include "XImage/XImage.hpp"
//#include "XText.h"
namespace XResource
{
	//struct XPoint
	//{
	//	XPoint()
	//	{
	//	}
	//	XPoint(double x, double y)
	//	{
	//		mX = x;
	//		mY = y;
	//	}
	//	XPoint(int x, int y)
	//	{
	//		mX = x;
	//		mY = y;
	//	}
	//	double mX, mY;
	//};
    struct XColor {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };
    class XUIColor {
    public:
        XUIColor() {
            _color.r = 255;
            _color.g = 255;
            _color.b = 255;
            _color.a = 255;
        }
        XUIColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) {
            _color.r = r;
            _color.g = g;
            _color.b = b;
            _color.a = a;
        }
        XColor _color;
        static std::shared_ptr<XUIColor> uiColor(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a) {
            std::shared_ptr<XUIColor> color(new XUIColor(r, g, b, a));
            return color;
        }
        static std::shared_ptr<XUIColor> uiColor(const unsigned char r, const unsigned char g, const unsigned char b) {
            std::shared_ptr<XUIColor> color(new XUIColor(r, g, b, 255));
            return color;
        }
    };
}





