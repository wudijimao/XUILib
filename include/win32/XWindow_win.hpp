#pragma once

#include "core\stdafx.hpp"
#include "core\XWindow.hpp"

#ifdef TARGET_OS_MSWINDOWS

#endif // TARGET_OS_MSWINDOWS

class WindowsManager;
namespace XDUILib
{
	class XApp;
}
class SIMPLEDIRECTUI_API_DEBUG XWindow_win : public XWindow {
public:
	XWindow_win();
	virtual void showInFront() override;
public://protected
	bool  init(PSTR szCmdLine, int iCmdShow); //set friend to XApp not Work?
protected:
	friend class WindowsManager;
	friend class XApp;
	LRESULT RealWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT processMouseEvent(UINT iMsg, WPARAM wParam, LPARAM lParam);
private:
	HWND mHwnd;
};