#include "../../include/core/stdafx.hpp"
#include "GDICacheGraphics.hpp"

using namespace Gdiplus;

void GDICacheGraphics::Creat(const XResource::XRect& rect)
{
	mBitmap = new Gdiplus::Bitmap(rect.Width(), rect.Height());
	mBkgGraphics = Graphics::FromImage(mBitmap);
}
GDICacheGraphics::~GDICacheGraphics()
{
	delete mBitmap;
}