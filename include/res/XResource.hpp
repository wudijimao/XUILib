#pragma once
#include "../core/stdafx.hpp"
#include "XRect.hpp"
#include "XRectPro.hpp"
#include "XImage/XImage.hpp"
#include "XString.hpp"
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
    class SIMPLEDIRECTUI_API XUIColor {
    public:
        static std::shared_ptr<XUIColor>whiteColor() {
            static auto color = std::make_shared<XUIColor>(255,255,255,255);
            return color;
        }
        static std::shared_ptr<XUIColor>clearColor() {
            static auto color = std::make_shared<XUIColor>(0,0,0,0);
            return color;
        }
        static std::shared_ptr<XUIColor>redColor() {
            static auto color = std::make_shared<XUIColor>(254,66,29,255);
            return color;
        }
        static std::shared_ptr<XUIColor>greenColor() {
            static auto color = std::make_shared<XUIColor>(41,249,151,255);
            return color;
        }
        static std::shared_ptr<XUIColor>blueColor() {
            static auto color = std::make_shared<XUIColor>(34,171,248,255);
            return color;
        }
        static std::shared_ptr<XUIColor>yellowColor() {
            static auto color = std::make_shared<XUIColor>(255,214,68,255);
            return color;
        }
        static std::shared_ptr<XUIColor>grayColor() {
            static auto color = std::make_shared<XUIColor>(198,198,198,255);
            return color;
        }
        static std::shared_ptr<XUIColor>lightGrayColor() {
            static auto color = std::make_shared<XUIColor>(218,218,218,255);
            return color;
        }
        static std::shared_ptr<XUIColor>pinkColor() {
            static auto color = std::make_shared<XUIColor>(255,192,203,255);
            return color;
        }
        std::shared_ptr<XUIColor> copy() {
            return std::make_shared<XUIColor>(*this);
        }
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





