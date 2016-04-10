#include "core\stdafx.hpp"
#include "core\GLCanvas.hpp"
#include "GDIRender.hpp"
#include <gdiplus.h>

class SIMPLEDIRECTUI_API_DEBUG GLCanvas_ios : public GLCanvas
{
public:
	~GLCanvas_ios() {
		::ReleaseDC(mHWND, mHDC);
	}
	HWND mHWND = nullptr;
	HDC mHDC = nullptr;
	bool init(HWND hWnd) {
		RECT rct;
		GetWindowRect(mHWND, &rct);
		setSize(XResource::XSize(rct.right - rct.left, rct.bottom - rct.top));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mHWND = hWnd;
		mHDC = ::GetDC(hWnd);
		initMemDC();
		CreateDIB();
		initOpengl(mDIB_DC);
		return true;
	}
	virtual bool Present() {
		this->GLCanvas::Present();
		//test Opengl
		//glClearColor(1, 0, 1, 1);
		//glClear(GL_COLOR_BUFFER_BIT);

		//TODO::drawtowin by gdi+
		//XResource::XColor *piexls = new XResource::XColor[_size.Width() * _size.Height()];
		//SwapBuffers(mDIB_DC);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, _size.Width(), _size.Height(), GL_BGRA, GL_UNSIGNED_BYTE, bmp_cnt);


		RECT rct;
		GetWindowRect(mHWND, &rct);
		POINT ptWinPos = { rct.left, rct.top };
		POINT    ptSrc = { 0, 0 };
		SIZE    sizeWindow = { _size.Width(), _size.Height() };

		BLENDFUNCTION    m_Blend;
		m_Blend.BlendOp = AC_SRC_OVER; //theonlyBlendOpdefinedinWindows2000
		m_Blend.BlendFlags = 0; //nothingelseisspecial...
		m_Blend.AlphaFormat = AC_SRC_ALPHA; //...
		m_Blend.SourceConstantAlpha = 255;//AC_SRC_ALPHA 
		UpdateLayeredWindow(mHWND, mHDC, &ptWinPos, &sizeWindow, mDIB_DC, &ptSrc, 0, &m_Blend, ULW_ALPHA);
		return true;
	}
private:
	HBITMAP mBitMap = nullptr;
	HDC mMemDC = nullptr;
	HDC mDIB_DC = nullptr;
	HBITMAP mDIBBitMap;
	unsigned char *bmp_cnt = nullptr;
	Gdiplus::Graphics *mBkgGraphics;

	bool initOpengl(HDC hdc) {
		int bits = 32;
		static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			//PFD_DRAW_TO_BITMAP |
			PFD_SUPPORT_OPENGL,				// Format Must Support OpenGL
			//PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			bits,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};
		int piexelFormat = ChoosePixelFormat(mHDC, &pfd);
		if (!SetPixelFormat(mHDC, piexelFormat, &pfd)) {
			return false;
		}

		_context = wglCreateContext(mHDC);

		if (!wglMakeCurrent(mHDC, _context)) {
			return false;
		}

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			return false;
		}

		//::ReleaseDC(hWnd, mHDC);//一定要释放句柄
		glGenRenderbuffers(1, &_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_SRGB8_ALPHA8, _size.Width(), _size.Height());
		if (!this->InitGLProgram()) {
			return false;
		}
		if (!this->InitFrameBuffer()) {
			return false;
		}
		return true;
	}
	void CreateDIB()
	{
		BITMAPINFOHEADER BIH;
		int iSize = sizeof(BITMAPINFOHEADER);
		memset(&BIH, 0, iSize);
		BIH.biSize = iSize;
		BIH.biWidth = _size.Width();
		BIH.biHeight = _size.Height();
		BIH.biPlanes = 1;
		BIH.biBitCount = 32;
		BIH.biCompression = BI_RGB;

		if (mDIB_DC)
			DeleteDC(mDIB_DC);
		if (mDIBBitMap)
			DeleteObject(mDIBBitMap);
		mDIB_DC = CreateCompatibleDC(NULL);
		
		
		mDIBBitMap = CreateDIBSection(
			mDIB_DC,
			(BITMAPINFO*)&BIH,
			DIB_RGB_COLORS,
			(void**)&bmp_cnt,
			NULL,
			0);
		if (mDIBBitMap) {
			SelectObject(mDIB_DC, mDIBBitMap);
		}
	}
	bool initMemDC() {
		static GDIPlusInitHelper helper;
		RECT rct;
		GetWindowRect(mHWND, &rct);
		POINT ptWinPos = { rct.left, rct.top };
		_size.Width(rct.right - rct.left);
		_size.Height(rct.bottom - rct.top);

		mMemDC = CreateCompatibleDC(mHDC);
		mBitMap = CreateCompatibleBitmap(mHDC, _size.Width(), _size.Height());
		SelectObject(mMemDC, mBitMap);//???
		mBkgGraphics = new Gdiplus::Graphics(mMemDC);
		mBkgGraphics->SetCompositingMode(Gdiplus::CompositingMode::CompositingModeSourceOver);
		mBkgGraphics->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);//抗锯齿，不开启画颜色会有问题
		mBkgGraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);//图片缩放质量

																				
		DWORD dwExStyle = GetWindowLong(mHWND, GWL_EXSTYLE);  // 设置层次窗口

		if ((dwExStyle & WS_EX_LAYERED) != WS_EX_LAYERED)
		{
			SetWindowLong(mHWND, GWL_EXSTYLE, dwExStyle ^ WS_EX_LAYERED);
		}
		mBkgGraphics->Clear(Gdiplus::Color(254, 0, 0, 0));
		return true;
	}
public:
	HGLRC _context;
};