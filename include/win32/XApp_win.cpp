#include "../core/XApp.hpp"
#include "../core/MutiThread/XRunLoop.hpp"
#include "XWindow_win.hpp"

std::shared_ptr<IXRunLoop> gXMainRunloop;
HWND mHwnd;
static const unsigned int RunLoopMsg = WM_USER + 100;

namespace XDUILib
{
	int XApp::run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
		init();
		if (_mainWindow == nullptr)
		{
			return -1;
		}
		gXMainRunloop = std::make_shared<XMainRunloop>();
		auto winWindow = std::dynamic_pointer_cast<XWindow_win>(_mainWindow);
		winWindow->init(szCmdLine, iCmdShow);
		mHwnd = winWindow->getHwnd();
		this->internalInit();
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (msg.message == RunLoopMsg)
			{
				getMainRunLoop()->_do();
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return msg.wParam;
	}
}
	

std::shared_ptr<IXRunLoop> getMainRunLoop() {
	return gXMainRunloop;
}

void XMainRunloop::run() {
}
bool XMainRunloop::weakUp(XRunLoopSource *source) {
	IXRunLoop::weakUp(source);
	PostMessage(mHwnd, RunLoopMsg, 0, 0);
	return true;
}
void XMainRunloop::wait() {
}
bool XMainRunloop::waitUntil(std::chrono::time_point<std::chrono::system_clock> &&in_time) {
	return false;
}
