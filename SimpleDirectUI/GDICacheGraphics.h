#pragma once
#include"GDIGraphics.h"

//×¨ÃÅ»º´æ
class SIMPLEDIRECTUI_API GDICacheGraphics : public GDIGraphics
{
public:
	void Creat(const XResource::XRect& rect);
	~GDICacheGraphics();
	Gdiplus::Bitmap& Bitmap()
	{
		return *mBitmap;
	}
private:
	Gdiplus::Bitmap *mBitmap = {NULL};
};

