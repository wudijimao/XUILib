#pragma once

#include "core\stdafx.hpp"
#include "core\XWindow.hpp"

#ifdef TARGET_OS_MSWINDOWS
struct MouseStatusStruct
{
	int X, Y;
	int LastX, LastY;
	void inline SetMouseStatus(WPARAM wParam, LPARAM lParam)
	{
		LastX = X;
		LastY = Y;
		X = (int)(short)LOWORD(lParam);
		Y = (int)(short)HIWORD(lParam);
		mWParam = wParam;
	}
	bool IsLButtonDown() const
	{
		return (mWParam & WM_LBUTTONDOWN) != 0;
	}
	bool IsRButtonDown() const
	{
		return (mWParam & WM_RBUTTONDOWN) != 0;
	}
	bool IsXButton1Down() const
	{
		return (mWParam & WM_XBUTTONDOWN) != 0;
	}
	bool IsXButton2Down() const
	{
		return (mWParam & WM_XBUTTONDOWN) != 0;
	}
	bool IsShiftDown() const
	{
		return (mWParam & MK_SHIFT) != 0;
	}
	bool IsContrlDown() const
	{
		return (mWParam & MK_CONTROL) != 0;
	}
	WPARAM mWParam;
};
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
private:
	HWND mHwnd;

	MouseStatusStruct mMouseStatus;
};