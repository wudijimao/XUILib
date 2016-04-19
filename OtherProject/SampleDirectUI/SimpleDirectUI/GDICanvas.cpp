#include "GDICanvas.h"

using namespace Gdiplus;


GDIPlusInitHelper::GDIPlusInitHelper()
{
	//初始化GDI+
	GdiplusStartup(&mGdiplusToken, &mGdiplusStartupInput, NULL);
}

GDIPlusInitHelper::~GDIPlusInitHelper()
{
	GdiplusShutdown(mGdiplusToken);

}

bool GDICanvas::Init(HWND hwnd) {
	mHwnd = hwnd;
	//初始化画布
	HDC hdc = ::GetDC(mHwnd);
	RECT rct;
	GetWindowRect(mHwnd, &rct);
	POINT ptWinPos = { rct.left, rct.top };
	mSize.Width(rct.right - rct.left);
	mSize.Height(rct.bottom - rct.top);

	mMemDC = CreateCompatibleDC(hdc);
	mBitMap = CreateCompatibleBitmap(hdc, mSize.Width(), mSize.Height());
	SelectObject(mMemDC, mBitMap);//???
	mBkgGraphics = new Gdiplus::Graphics(mMemDC);
	mBkgGraphics->SetCompositingMode(CompositingMode::CompositingModeSourceOver);
	mBkgGraphics->SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);//抗锯齿，不开启画颜色会有问题
	mBkgGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);//图片缩放质量

																			// 设置层次窗口
	DWORD dwExStyle = GetWindowLong(mHwnd, GWL_EXSTYLE);

	if ((dwExStyle & WS_EX_LAYERED) != WS_EX_LAYERED)
	{
		SetWindowLong(mHwnd, GWL_EXSTYLE, dwExStyle ^ WS_EX_LAYERED);
	}
	mBkgGraphics->Clear(Color(254, 0, 0, 0));

	/*	POINT    ptSrc = { 0, 0 };
	SIZE    sizeWindow = { mWidth, mHeight };

	BLENDFUNCTION    m_Blend;
	m_Blend.BlendOp = AC_SRC_OVER; //theonlyBlendOpdefinedinWindows2000
	m_Blend.BlendFlags = 0; //nothingelseisspecial...
	m_Blend.AlphaFormat = AC_SRC_ALPHA; //...
	m_Blend.SourceConstantAlpha = 255;//AC_SRC_ALPHA

	// 完成透明不规则窗口的绘制
	UpdateLayeredWindow(hwnd, hdc, &ptWinPos, &sizeWindow, mMemDC, &ptSrc, 0, &m_Blend, ULW_ALPHA);*/
	return true;
}
void GDICanvas::ReSize(int width, int height)
{
}

void GDICanvas::releaseRes() {
	if (mBkgGraphics) {
		mBkgGraphics->ReleaseHDC(mMemDC);
		DeleteObject(mBitMap);
		DeleteDC(mMemDC);
		//TODO:测试未调用Create时销毁会出不会崩溃
		//DeleteObject(mGraphics);
		DeleteObject(mBkgGraphics);
		mBkgGraphics = nullptr;
	}
}

void GDICanvas::Update() {
	HDC hdc = ::GetDC(mHwnd);
	RECT rct;
	GetWindowRect(mHwnd, &rct);
	POINT ptWinPos = { rct.left, rct.top };

	POINT    ptSrc = { 0, 0 };

	SIZE    sizeWindow = { mSize.Width(), mSize.Height() };

	BLENDFUNCTION    m_Blend;
	m_Blend.BlendOp = AC_SRC_OVER; //theonlyBlendOpdefinedinWindows2000
	m_Blend.BlendFlags = 0; //nothingelseisspecial...
	m_Blend.AlphaFormat = AC_SRC_ALPHA; //...
	m_Blend.SourceConstantAlpha = 255;//AC_SRC_ALPHA 
	UpdateLayeredWindow(mHwnd, hdc, &ptWinPos, &sizeWindow, mMemDC, &ptSrc, 0, &m_Blend, ULW_ALPHA);
}