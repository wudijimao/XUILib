#include "stdafx.h"
#include "GDICanvas.h"
#include "GDIRender.h"
using namespace Gdiplus;

#define TWO_PI (2.0 * 3.14159)
using namespace XResource;



void abc()
{
	int a = 0;
}
GDIRender::GDIRender()
{
}

GDIRender::~GDIRender()
{
}

void GDIRender::DrawBackGround(Gdiplus::Color &color, const XResource::XRect &r)
{
	Gdiplus::Color c = color;
	//分层窗口FillRectangle画255的透明度时会画成跟“正片叠底”相反的叫做“滤色”的特殊处理
	if (c.GetAlpha() == 255)
	{
		c = Gdiplus::Color(254, c.GetR(), c.GetB(), c.GetG());
	}
	Gdiplus::SolidBrush brush(c);
	mCavas->mBkgGraphics->FillRectangle(&brush, r.IntX(), r.IntY(), r.IntWidth(), r.IntHeight());
}
void GDIRender::DrawImage(Gdiplus::Bitmap &img, const XResource::XRect &rect, int x, int y)
{
	Gdiplus::Rect temp(rect.X(), rect.Y(), rect.Width(), rect.Height());
	mCavas->mBkgGraphics->DrawImage(&img, temp, x, y, rect.Width(), rect.Height(), Gdiplus::UnitPixel);
}
void GDIRender::DrawImage(XResource::XImage &xImg, const XResource::XRect &xRect)
{
	XRect r = xImg.GetDrawRect(xRect);
	mCavas->mBkgGraphics->DrawCachedBitmap(xImg.CacheImage(*mCavas->mBkgGraphics), r.X(), r.Y());
}

void GDIRender::DrawString(const std::wstring &text, const XResource::XRect &xRect)
{
	FontFamily  fontFamily(L"Times New Roman");
	Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	PointF      pointF(xRect.X(), xRect.Y());
	SolidBrush  solidBrush(Color(255, 0, 0, 255));

	mCavas->mBkgGraphics->DrawString(text.c_str(), -1, &font, pointF, &solidBrush);
}
void GDIRender::DrawString(const std::wstring &text, const XResource::XRect &xRect, Gdiplus::StringAlignment align)
{
	FontFamily   fontFamily(L"Arial");
	Font         font(&fontFamily, 12, FontStyleBold, UnitPoint);
	RectF        rectF(30.0f, 10.0f, 120.0f, 40.0f);
	StringFormat stringFormat;
	SolidBrush   solidBrush(Color(255, 0, 0, 255));

	// Center-justify each line of text.
	stringFormat.SetAlignment(StringAlignmentNear);

	// Center the block of text (top to bottom) in the rectangle.
	stringFormat.SetLineAlignment(StringAlignmentNear);

	stringFormat.SetTrimming(StringTrimmingNone);

	mCavas->mBkgGraphics->DrawString(text.c_str(), -1, &font, rectF, &stringFormat, &solidBrush);

}
void GDIRender::DrawString(const XResource::XText &text, const XResource::XRect &xRect)
{
	FontFamily   fontFamily(text.mFont.c_str());
	Font         font(&fontFamily, text.FontSize, text.mFontStyle, UnitPoint);
	StringFormat stringFormat;
	stringFormat.SetAlignment(StringAlignmentNear);
	stringFormat.SetLineAlignment(StringAlignmentNear);
	stringFormat.SetTrimming(StringTrimmingNone);

	Gdiplus::Color c = text.mColor;
	//分层窗口FillRectangle画255的透明度时会画成跟“正片叠底”相反的叫做“滤色”的特殊处理
	if (c.GetAlpha() == 255)
	{
		c = Gdiplus::Color(254, c.GetR(), c.GetB(), c.GetG());
	}
	SolidBrush   solidBrush(c);
	RectF rect(xRect.X(), xRect.Y(), xRect.Width(), xRect.Height());

	mCavas->mBkgGraphics->DrawString(text.mText.c_str(), -1, &font, rect, &stringFormat, &solidBrush);
}


XRect GDIRender::MeasureString(const std::wstring &text)
{
	RectF temp;
	FontFamily fontFamily(L"Arial");
	Font font(&fontFamily, 12, FontStyleBold, UnitPoint);
	mCavas->mBkgGraphics->MeasureString(text.c_str(), -1, &font, RectF(0, 0, 1000, 1000), &temp);
	return XRect(temp.X, temp.Y, temp.Width, temp.Height);
}

XRect GDIRender::MeasureString(const XResource::XText &text, const XResource::XRect &xRect)
{
	FontFamily   fontFamily(text.mFont.c_str());
	Font         font(&fontFamily, text.FontSize, text.mFontStyle, UnitPoint);
	StringFormat stringFormat;
	stringFormat.SetAlignment(StringAlignmentNear);
	stringFormat.SetLineAlignment(StringAlignmentNear);
	stringFormat.SetTrimming(StringTrimmingNone);
	SolidBrush solidBrush(text.mColor);

	RectF rect(xRect.X(), xRect.Y(), xRect.Width(), xRect.Height());

	RectF temp;
	mCavas->mBkgGraphics->MeasureString(text.mText.c_str(), -1, &font, rect, &stringFormat, &temp);
	return XRect(temp.X, temp.Y, temp.Width, temp.Height);
}

bool GDIRender::Init(IXCanvas *canvas)
{
	if (canvas->GetType() == RenderType::GDIPlus)
	{
		//TODO::或许应该使用智能指针！！！！
		mCavas = dynamic_cast<GDICanvas*>(canvas);
		return true;
	}
	return false;
}
