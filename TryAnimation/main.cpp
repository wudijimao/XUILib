#include <windows.h>
#include <string>
#include <assert.h>
#include <gdiplus.h>
#include <time.h>
using namespace std;
using namespace Gdiplus;

HWND mHwnd;
void InitMemDC();
void InitDC();
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	if (iMsg == WM_CREATE)
	{
		InitMemDC();
		InitDC();
		return 1;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

bool CreateWnd(HINSTANCE hInstance, int iCmdShow)
{
	wstring appNameClass = L"appNameClass";

	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW/* | WS_EX_TOOLWINDOW | WS_EX_TOPMOST*/;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = appNameClass.c_str();
	wndclass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);


	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			appNameClass.c_str(), MB_ICONERROR);
		DWORD a = ::GetLastError();
		return false;
	}

	mHwnd = CreateWindowEx(
		/*WS_EX_TOPMOST | */WS_EX_LAYERED,
		appNameClass.c_str(),
		L"aaa",
		WS_POPUP | WS_MINIMIZEBOX,
		100,
		100,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (!::IsWindow(mHwnd))
	{
		DWORD a = ::GetLastError();
		assert("未成功创建窗口" && 0);
		return false;
	}
	ShowWindow(mHwnd, iCmdShow);
	UpdateWindow(mHwnd);
	//SetTimer(mHwnd, 0, 10, NULL);
	return true;
}

HDC mMemDC;
HDC hdc;
RECT rct;
POINT ptWinPos;
POINT    ptSrc;
SIZE    sizeWindow;
BLENDFUNCTION    m_Blend;
void Print()
{
	UpdateLayeredWindow(mHwnd, hdc, &ptWinPos, &sizeWindow, mMemDC, &ptSrc, 0, &m_Blend, ULW_ALPHA);
}

void InitDC()
{
	hdc = ::GetDC(mHwnd);
	GetWindowRect(mHwnd, &rct);
	ptWinPos = { rct.left, rct.top };
	ptSrc = { 0, 0 };
	sizeWindow = { 800, 600 };
	m_Blend.BlendOp = AC_SRC_OVER; //theonlyBlendOpdefinedinWindows2000
	m_Blend.BlendFlags = 0; //nothingelseisspecial...
	m_Blend.AlphaFormat = AC_SRC_ALPHA; //...
	m_Blend.SourceConstantAlpha = 255;//AC_SRC_ALPHA
}

Gdiplus::Color c,a;
Gdiplus::SolidBrush *brush, *backBrush;
Gdiplus::Graphics *mBkgGraphics;
Bitmap *mBitmap;
HBITMAP mBitMap;
int isOK = false;
int x, y, width, height;
Gdiplus::GdiplusStartupInput mGdiplusStartupInput;
ULONG_PTR mGdiplusToken;
void InitMemDC()
{
	GdiplusStartup(&mGdiplusToken, &mGdiplusStartupInput, NULL);
	a = Gdiplus::Color(0, 0, 0, 0);
	c = Gdiplus::Color(254, 255, 0, 0);
	backBrush = new Gdiplus::SolidBrush(a);
	brush = new Gdiplus::SolidBrush(c);

	//mBitmap = new Gdiplus::Bitmap(800, 600);
	//mBkgGraphics = Graphics::FromImage(mBitmap);

	HDC hdc = ::GetDC(mHwnd);
	mMemDC = CreateCompatibleDC(hdc);
	mBitMap = CreateCompatibleBitmap(hdc, 800, 600);
	SelectObject(mMemDC, mBitMap);//???
	mBkgGraphics = new Gdiplus::Graphics(mMemDC);

	// 设置层次窗口
	DWORD dwExStyle = GetWindowLong(mHwnd, GWL_EXSTYLE);

	if ((dwExStyle & WS_EX_LAYERED) != WS_EX_LAYERED)
	{
		SetWindowLong(mHwnd, GWL_EXSTYLE, dwExStyle ^ WS_EX_LAYERED);
	}
	x = 0;
	y = 0;
	width = 20;
	height = 20;
	isOK = true;
	mBkgGraphics->SetCompositingMode(Gdiplus::CompositingMode::CompositingModeSourceCopy);
}

int xIncreace = 1;
void Tick()
{
	if (x > 780)
	{
		xIncreace = -1;
	}
	else if (x < 0)
	{
		xIncreace = 1;
	}
	x += xIncreace;
	//mBkgGraphics->Clear(a);
	//mBkgGraphics->FillRectangle(backBrush, 0, 0, 800, 600);
	
	mBkgGraphics->FillRectangle(backBrush, x - xIncreace, y, width, height);
	mBkgGraphics->FillRectangle(brush, x, y, width, height);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	CreateWnd(hInstance, iCmdShow);
	MSG msg;
	long t1 = clock();
	int num = 0;
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			else if (msg.message == WM_SYSCOMMAND)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 完成某些工作的其它行程序
			if (isOK)
			{
				Tick();
				Print();
				++num;
			}
		}
	}
	long t2 = clock();
	int min = t2 - t1;
	double frekunce = num / (t2 / 1000.0);
	return msg.wParam;
}