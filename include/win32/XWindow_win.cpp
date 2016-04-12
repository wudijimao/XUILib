#include "XWindow_win.hpp"
#include "res\XResource.hpp"
#include "WindowsManager.hpp"
#include "XCanvas_win.hpp"
#include "core\GLRender.hpp"

#include "../core/input/XMouse.hpp"



std::shared_ptr<IXWindow> IXWindow::createWindow() {
	return std::make_shared<XWindow_win>();
}

void XWindow_win::showInFront() {
	//TODO impl
}

XWindow_win::XWindow_win() {
}

bool XWindow_win::init(PSTR szCmdLine, int iCmdShow) {
	HINSTANCE hInstance = GetModuleHandle(NULL);
	std::string appNameClass = "appNameClass";
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW/* | WS_EX_TOOLWINDOW | WS_EX_TOPMOST*/;
	wndclass.lpfnWndProc = WindowsManager::WndProc;
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
		return NULL;
	}

	mHwnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_LAYERED,
		appNameClass.c_str(),
		"",
		WS_POPUP | WS_MINIMIZEBOX,
		_rect.X(),
		_rect.Y(),
		_rect.Width(),
		_rect.Height(),
		NULL,
		NULL,
		hInstance,
		this);
	if (!::IsWindow(mHwnd))
	{
		DWORD a = ::GetLastError();
		return NULL;
	}
	ShowWindow(mHwnd, iCmdShow);
	UpdateWindow(mHwnd);

	/*//创建光标 分层窗口不能用 只能自绘,但是是否影响输入法的位置呢？？
	if (!CreateCaret(mHwnd, NULL, 500, 500))
	{
		assert(L"创建光标失败");
	}*/
	return true;
}

LRESULT XWindow_win::RealWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int addMs = 0;
	switch (iMsg)
	{
	case WM_TIMER:
		this->update();
		/*addMs += TimerMS;
		if (addMs >= CaretShowMs)
		{
			addMs = 0;
			GlobalStatus::GetInstance().IsCaretShow(!GlobalStatus::GetInstance().IsCaretShow());
		}
		OnDraw();*/
		return 0;
	case WM_CREATE: {
		auto canvas = std::make_shared<GLCanvas_ios>();
		canvas->init(hwnd);
		_canvas = canvas;
		_render = std::make_shared<GLRender>();
		_render->Init(_canvas.get());
		SetTimer(hwnd, 0, 1, NULL);
	}
					return 0;
	case WM_SIZE: {
		XResource::XSize size(LOWORD(lParam), HIWORD(lParam));
		_canvas->setSize(size);
	}
				  return 0;
	case WM_PAINT:
		//系统托管
		return 0;
	case WM_DESTROY:
		WindowsManager::GetInstanc().UnRegist(mHwnd);
		PostQuitMessage(0);
		return 0;
	case WM_ERASEBKGND:
		return 0;

	case WM_LBUTTONDOWN:
		SetCapture(hwnd);
	case WM_LBUTTONUP:
		ReleaseCapture();
		//鼠标滚轮事件
	case WM_MOUSEWHEEL:
	case WM_MOUSEMOVE:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_XBUTTONUP:
	case WM_XBUTTONDOWN:
		return processMouseEvent(iMsg, wParam, lParam);
	case WM_KEYDOWN:
	{
		//TODO::Key
		return 0;
	}
	//窗体移动事件
	case WM_MOVE:
		//mRect.X((int)(short)LOWORD(lParam));
		//mRect.Y((int)(short)HIWORD(lParam));
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

LRESULT XWindow_win::processMouseEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	XMouse *mouse = new XMouse();
	mouse->mPosition.X(LOWORD(lParam));
	mouse->mPosition.Y(HIWORD(lParam));
	switch (iMsg)
	{
	case WM_LBUTTONDOWN:
		mouse->eventButton = MouseEventButton::Left;
		mouse->eventType = MouseEventType::Down;
		break;
	case WM_LBUTTONUP:
		mouse->eventButton = MouseEventButton::Left;
		mouse->eventType = MouseEventType::Up;
		break;
	case WM_RBUTTONDOWN:
		mouse->eventButton = MouseEventButton::Right;
		mouse->eventType = MouseEventType::Down;
		break;
	case WM_RBUTTONUP:
		mouse->eventButton = MouseEventButton::Right;
		mouse->eventType = MouseEventType::Up;
		break;
	case WM_MOUSEMOVE:
		mouse->eventType = MouseEventType::Move;
		break;
	//case WM_LBUTTONDBLCLK:
	//case WM_MOUSEWHEEL:
	}
	//TODO::processinput
	this->input(mouse, 1);
	this->dispatchMouseEvents();
	return 0;
}
