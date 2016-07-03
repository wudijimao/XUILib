#pragma once
#include "stdafx.hpp"
#define GLEW_STATIC
#include <GL\glew.h>

class GLHelper
{
public:
	GLHelper();
	~GLHelper();
	static bool winInit(HWND hWnd);
	static bool init();
private:

};

GLHelper::GLHelper()
{
}

GLHelper::~GLHelper()
{
}

bool GLHelper::winInit(HWND hWnd) {
	//windows
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

	HGLRC context = wglCreateContext(hdc);
	if (!wglMakeCurrent(hdc, context)) {
		return false;
	}
	::ReleaseDC(hWnd, hdc);//一定要释放句柄
	return init();
}

bool GLHelper::init() {
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		return false;
	}

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	return true;
}