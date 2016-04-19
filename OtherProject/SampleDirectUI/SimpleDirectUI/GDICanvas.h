#pragma once
#include "IXCanvas.h"
#include <windows.h>
#include <gdiplus.h>



class GDIPlusInitHelper
{
public:
	GDIPlusInitHelper();
	~GDIPlusInitHelper();
private:
	Gdiplus::GdiplusStartupInput mGdiplusStartupInput;
	ULONG_PTR mGdiplusToken;
};



class GDIRender;
class GDICanvas : public IXCanvas
{
public:
	friend class GDIRender;
	virtual RenderType GetType()
	{
		return RenderType::GDIPlus;
	}
	virtual XResource::XSize GetSize() {
		return mSize;
	}
	GDICanvas() {
		static GDIPlusInitHelper sGdiPlus;
	}
	~GDICanvas() {
		releaseRes();
	}

	bool Init(HWND hwnd);
	void ReSize(int width, int height);

	void releaseRes();

	virtual void Update();
protected:
	Gdiplus::Graphics *mBkgGraphics = nullptr;
private:
	XResource::XSize mSize;
	HWND mHwnd;
	HDC mMemDC;
	HBITMAP mBitMap;
};
