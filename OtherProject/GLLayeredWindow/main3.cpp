#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/glu.h>

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

#include <assert.h>
#include <tchar.h>

#ifdef  assert
#define verify(expr) if(!expr) assert(0)
#else verify(expr) expr
#endif

const TCHAR szAppName[] = _T("TransparentGL");
const TCHAR wcWndName[] = _T("WS_EX_LAYERED OpenGL");

HDC hDC;
HGLRC m_hrc;
int w(240);
int h(240);

HDC pdcDIB;
HBITMAP hbmpDIB;
BYTE *bmp_cnt(NULL);
int cxDIB(0);
int cyDIB(0);
BITMAPINFOHEADER BIH;
HWND g_hWnd;


BOOL initSC()
{
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 0);

	return 0;
}

void resizeSC(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

BOOL renderSC()
{
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	
	glBegin(GL_TRIANGLES);                              // Drawing Using Triangles
	glColor3f(0, 1, 1);
	glVertex3f(0.0f, 1.0f, 0.0f);                  // Top
	glColor3f(0.0f, 1.0f, 0.0f);                      // Set The Color To Green
	glVertex3f(-1.0f, -1.0f, 0.0f);                  // Bottom Left
	glColor3f(0.0f, 0.0f, 1.0f);                      // Set The Color To Blue
	glVertex3f(1.0f, -1.0f, 0.0f);                  // Bottom Right
	glEnd();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPopMatrix();
	glFlush();

	return 0;
}

// DIB -> hDC
void draw(HDC pdcDest)
{
	assert(pdcDIB);

	verify(BitBlt(pdcDest, 0, 0, w, h, pdcDIB, 0, 0, SRCCOPY));
}

void CreateDIB(int cx, int cy)
{
	assert(cx > 0);
	assert(cy > 0);

	cxDIB = cx;
	cyDIB = cy;

	int iSize = sizeof(BITMAPINFOHEADER);
	memset(&BIH, 0, iSize);

	BIH.biSize = iSize;
	BIH.biWidth = cx;
	BIH.biHeight = cy;
	BIH.biPlanes = 1;
	BIH.biBitCount = 32;
	BIH.biCompression = BI_RGB;

	if (pdcDIB)
		verify(DeleteDC(pdcDIB));

	pdcDIB = CreateCompatibleDC(NULL);
	assert(pdcDIB);

	if (hbmpDIB)
		verify(DeleteObject(hbmpDIB));

	hbmpDIB = CreateDIBSection(
		pdcDIB,
		(BITMAPINFO*)&BIH,
		DIB_RGB_COLORS,
		(void**)&bmp_cnt,
		NULL,
		0);

	assert(hbmpDIB);
	assert(bmp_cnt);

	if (hbmpDIB)
		SelectObject(pdcDIB, hbmpDIB);
}

BOOL CreateHGLRC()
{
	DWORD dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_BITMAP;

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = dwFlags;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int PixelFormat = ChoosePixelFormat(pdcDIB, &pfd);
	if (PixelFormat == 0) {
		assert(0);
		return FALSE;
	}

	BOOL bResult = SetPixelFormat(pdcDIB, PixelFormat, &pfd);
	if (bResult == FALSE) {
		assert(0);
		return FALSE;
	}

	m_hrc = wglCreateContext(pdcDIB);
	if (!m_hrc) {
		assert(0);
		return FALSE;
	}

	return TRUE;
}

LRESULT CALLBACK WindowFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_ERASEBKGND:
		return 0;
		break;

	case WM_CREATE:
		g_hWnd = hWnd;
		break;

	case WM_DESTROY:
		if (m_hrc)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_hrc);
		}
		PostQuitMessage(0);
		break;

	case WM_PAINT:

		break;

	case WM_SIZE:
		w = LOWORD(lParam); h = HIWORD(lParam);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hrc);

		CreateDIB(w, h);
		CreateHGLRC();
		verify(wglMakeCurrent(pdcDIB, m_hrc));

		initSC();
		resizeSC(w, h);
		renderSC();
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

void ImagePreMultAlpha()
{
	// The per pixel alpha blending API for layered windows deals with
	// pre-multiplied alpha values in the RGB channels. For further details see
	// the MSDN documentation for the BLENDFUNCTION structure. It basically
	// means we have to multiply each red, green, and blue channel in our image
	// with the alpha value divided by 255.
	//
	// Notes:
	// 1. ImagePreMultAlpha() needs to be called before every call to
	//    UpdateLayeredWindow() (in the RedrawLayeredWindow() function).
	//
	// 2. Must divide by 255.0 instead of 255 to prevent alpha values in range
	//    [1, 254] from causing the pixel to become black. This will cause a
	//    conversion from 'float' to 'BYTE' possible loss of data warning which
	//    can be safely ignored.

	BYTE *pPixel = NULL;

	
		// This is a special case. When the image width is already a multiple
		// of 4 the image does not require any padding bytes at the end of each
		// scan line. Consequently we do not need to address each scan line
		// separately. This is much faster than the below case where the image
		// width is not a multiple of 4.

		int totalBytes = w * h * 4;

		for (int i = 0; i < totalBytes; i += 4)
		{
			pPixel = bmp_cnt + i;
			//pPixel[0] = 255;// (BYTE)(pPixel[0] * (float)pPixel[3] / 255.0f);
			//pPixel[1] = 255;// (BYTE)(pPixel[1] * (float)pPixel[3] / 255.0f);
			//pPixel[2] = 255;// (BYTE)(pPixel[2] * (float)pPixel[3] / 255.0f);
			pPixel[3] = 255;
		}
}

void RedrawLayeredWindow()
{
	// The call to UpdateLayeredWindow() is what makes a non-rectangular
	// window possible. To enable per pixel alpha blending we pass in the
	// argument ULW_ALPHA, and provide a BLENDFUNCTION structure filled in
	// to do per pixel alpha blending.

	HDC hdc = GetDC(g_hWnd);

	if (hdc)
	{
		HGDIOBJ hPrevObj = 0;
		POINT ptDest = { 0, 0 };
		POINT ptSrc = { 0, 0 };
		SIZE client = { w, h };
		BLENDFUNCTION blendFunc = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

		//hPrevObj = SelectObject(pdcDIB, hbmpDIB);
		ClientToScreen(g_hWnd, &ptDest);

		UpdateLayeredWindow(g_hWnd, hdc, &ptDest, &client,
			pdcDIB, &ptSrc, 0, &blendFunc, ULW_ALPHA);

		//SelectObject(pdcDIB, hPrevObj);
		ReleaseDC(g_hWnd, hdc);
	}
}

int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR str, int nWinMode)
{
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowFunc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hThisInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName = szAppName;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, _T("RegisterClassEx - failed"), _T("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	HWND hWnd = CreateWindowEx(WS_EX_LAYERED, szAppName, wcWndName,
		WS_VISIBLE | WS_POPUP, 200, 150, w, h,
		NULL, NULL, hThisInst, NULL);
	if (!hWnd) {
		MessageBox(NULL, _T("CreateWindowEx - failed"), _T("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	//verify(SetLayeredWindowAttributes(hWnd, 0x0, 0, LWA_COLORKEY));

	MSG msg;
	while (1)
	{
		while (true)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				renderSC(); // OpenGL -> DIB
				ImagePreMultAlpha();
				RedrawLayeredWindow();
			}
		}
	}
	return (FALSE);
}
