#include "GLRender.h"
#include "GLHelper.h"
#include <vector>
#include <time.h>
#include <iterator>

struct GLColor {
	BYTE r = '\0';
	BYTE g = '\0';
	BYTE b = '\0';
	BYTE a = '\0';
};

void creatFramBuf() {
	GLuint colorBuf;
	glGenRenderbuffers(1, &colorBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, colorBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_SRGB8_ALPHA8, 100, 100);
	GLuint frameBuf;
	glGenFramebuffers(1, &frameBuf);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuf);

	int ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	GLColor piexls[100 * 100];
	if (ret == GL_FRAMEBUFFER_COMPLETE)
	{
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glReadPixels(0, 0, 100, 100, GL_RGBA, GL_UNSIGNED_BYTE, piexls);
	}
}

GLRender::GLRender(HWND hwnd) {
	mHwnd = hwnd;
	
}

bool GLRender::Init(IXCanvas *canvas) {
	return false;
}
Gdiplus::GdiplusStartupInput mGdiplusStartupInput;
ULONG_PTR mGdiplusToken;
void GLRender::Creat() {
	GdiplusStartup(&mGdiplusToken, &mGdiplusStartupInput, NULL);
	GLHelper::init();
	HDC hdc = ::GetDC(mHwnd);
	RECT rct;
	GetWindowRect(mHwnd, &rct);
	POINT ptWinPos = { rct.left, rct.top };
	mMemDC = hdc;// CreateCompatibleDC(hdc);
	//mBitMap = CreateCompatibleBitmap(hdc, rct.right - rct.left, rct.bottom - rct.top);
	//SelectObject(mMemDC, mBitMap);//???
								  //GLHelper::winInit(mMemDC);

								  // 设置层次窗口
	DWORD dwExStyle = GetWindowLong(mHwnd, GWL_EXSTYLE);

	if ((dwExStyle & WS_EX_LAYERED) != WS_EX_LAYERED)
	{
		SetWindowLong(mHwnd, GWL_EXSTYLE, dwExStyle ^ WS_EX_LAYERED);
	}
	GLHelper::winInit(mMemDC);
	//creatFramBuf();
}

void GLRender::ReSize(int width, int height) {

}
void GLRender::Paint() {
/*	for (int i = 0; i < 40000; i++)
	{
		((char*)(Bitmap.bmBits))[i] = 255;
	}*/

	HDC hdc = ::GetDC(mHwnd);
	RECT rct;
	GetWindowRect(mHwnd, &rct);
	POINT ptWinPos = { rct.left, rct.top };

	POINT    ptSrc = { 0, 0 };

	SIZE    sizeWindow = { rct.right - rct.left, rct.bottom - rct.top };
	
	
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

	wglSwapIntervalEXT(1);

	BLENDFUNCTION    m_Blend;
	m_Blend.BlendOp = AC_SRC_OVER; //theonlyBlendOpdefinedinWindows2000
	m_Blend.BlendFlags = 0; //nothingelseisspecial...
	m_Blend.AlphaFormat = AC_SRC_ALPHA; //...
	m_Blend.SourceConstantAlpha = 255;//AC_SRC_ALPHA 
	bool ret = UpdateLayeredWindow(mHwnd, NULL, &ptWinPos, &sizeWindow, hdc, &ptSrc, 0, &m_Blend, ULW_ALPHA);
	
}
void GLRender::Destory() {

}
void GLRender::DrawBackGround(Gdiplus::Color &color, const XResource::XRect &xRect) {

}
void GLRender::DrawLine(int x1, int y1, int x2, int y2) {

}
//需要绘制的矩形区域
void GLRender::DrawImage(XResource::XImage &img, const XResource::XRect &rect) {

}
//文本相关要单独处理？？ 如何与普通图形分开 又不过多多谢代码（分开的理由是OpenGl应该也会使用其渲染文本， 或者使用其他文字渲染引擎，比如FreeType/DirectWrite）
void GLRender::DrawString(const std::wstring &text, const XResource::XRect &xRect) {

}
void GLRender::DrawString(const std::wstring &text, const XResource::XRect &xRect, Gdiplus::StringAlignment align) {

}
void GLRender::DrawString(const XResource::XText &text, const XResource::XRect &xRect) {

}
XResource::XRect GLRender::MeasureString(const XResource::XText &text, const XResource::XRect &xRect) {
	return xRect;
}
XResource::XRect GLRender::MeasureString(const std::wstring &text) {
	return XResource::XRect(0, 0, 0, 0);
}