#include "core\stdafx.hpp"
#include "core\GLCanvas.hpp"

class GLCanvas_ios : public GLCanvas
{
public:
	~GLCanvas_ios() {
	}
	bool init(HWND hWnd) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		HDC hdc;
		hdc = ::GetDC(hWnd);
		int bits = 32;
		static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
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
		int piexelFormat = ChoosePixelFormat(hdc, &pfd);
		if (!SetPixelFormat(hdc, piexelFormat, &pfd)) {
			return false;
		}

		_context = wglCreateContext(hdc);
		if (!wglMakeCurrent(hdc, _context)) {
			return false;
		}
		::ReleaseDC(hWnd, hdc);//一定要释放句柄

		glGenRenderbuffers(1, &_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_SRGB8_ALPHA8, 100, 100);
		if (!this->InitGLProgram()) {
			return false;
		}
		if (!this->InitFrameBuffer()) {
			return false;
		}		return true;
	}
	bool Present() {
		this->GLCanvas::Present();

		//TODO::drawtowin by gdi+
		return true;
	}
public:
	HGLRC _context;
};