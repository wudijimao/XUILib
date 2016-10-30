#include "XWindow_win.hpp"
#include "res\XResource.hpp"
#include "WindowsManager.hpp"
#include "XCanvas_win.hpp"
#include "core\GLRender.hpp"
#include "../../include/core/UIResponder.hpp"
#include "../../include/core/input/XMouse.hpp"



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
		rect().X(),
		rect().Y(),
		rect().Width(),
		rect().Height(),
		NULL,
		NULL,
		hInstance,
		this);
	if (!::IsWindow(mHwnd))
	{
		DWORD a = ::GetLastError();
		return NULL;
	}
	initFinished();
	ShowWindow(mHwnd, iCmdShow);
	UpdateWindow(mHwnd);
	return true;
}

LRESULT XWindow_win::RealWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int addMs = 0;
	switch (iMsg)
	{
	case WM_TIMER:
		this->update();
		return 0;
	case WM_CREATE: {
		SetTimer(hwnd, 0, 16, NULL);
	}
					return 0;
	case WM_SIZE: {
		XResource::XSize size(LOWORD(lParam), HIWORD(lParam));
		auto canvas = std::make_shared<GLCanvas_ios>();
		canvas->init(hwnd, size);
		_canvas = canvas;
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
	case WM_KEYUP:
		break;
	//窗体移动事件
	case WM_MOVE:
		//mRect.X((int)(short)LOWORD(lParam));
		//mRect.Y((int)(short)HIWORD(lParam));
		return 0;
	case WM_IME_SETCONTEXT:{
			// return 0;  <== これじゃダメ  
			lParam &= ~ISC_SHOWUIALL;
			break;
		}
	case WM_CHAR:
		if (XUI::UIResponder::sFirstResponder != nullptr) {
			switch (wParam) {
			case VK_RETURN:
				break;
			case VK_BACK:
				XUI::UIResponder::sFirstResponder->deleteBackward();
				break;
			default: {
				char text[2];
				text[0] = (char)wParam;
				text[1] = '\0';
				XUI::UIResponder::sFirstResponder->insertText(text);
			}
			}
		}
		break;
	}
	
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

LRESULT XWindow_win::processMouseEvent(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	auto mouse = std::make_shared<XMouse>();
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
	this->input(mouse);
	this->dispatchMouseEvents();
	return 0;
}
