#include "../core/XApp.hpp"
#include "XWindow_win.hpp"

namespace XDUILib
{
	int XApp::run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
		init();
		if (_mainWindow == nullptr)
		{
			return -1;
		}
		auto winWindow = std::dynamic_pointer_cast<XWindow_win>(_mainWindow);
		winWindow->init(szCmdLine, iCmdShow);
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return msg.wParam;
	}
}