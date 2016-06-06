#include "../core/XApp.hpp"
#include "XWindow_win.hpp"
#include "XMainRunLoop_win32.hpp"

namespace XDUILib
{
	int XApp::run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
        _thisApp = this;
		init();
		if (_mainWindow == nullptr)
		{
			return -1;
		}
		auto winWindow = std::dynamic_pointer_cast<XWindow_win>(_mainWindow);
		winWindow->init(szCmdLine, iCmdShow);
		this->internalInit();
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!handleRunLoopMsg(msg.message)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return msg.wParam;
	}
}
	