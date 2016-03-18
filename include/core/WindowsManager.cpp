#include "stdafx.h"
#include "WindowsManager.h"
using namespace std;
LRESULT CALLBACK WindowsManager::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//TODO:需要加上线程同步
	if (iMsg == WM_NCCREATE)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		XWindow* creatingWin = static_cast<XWindow*>(lpcs->lpCreateParams);
		GetInstanc().Regist(hwnd, creatingWin);
	}
	else
	{
		std::map<HWND, XWindow*>::iterator iter = GetInstanc().mWndMap.find(hwnd);
		if (iter != GetInstanc().mWndMap.end())
		{
			return iter->second->RealWndProc(hwnd, iMsg, wParam, lParam);
		}
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
WindowsManager& WindowsManager::GetInstanc()
{
	static WindowsManager manager;
	return manager;
}
bool WindowsManager::Regist(HWND hwnd, XWindow* win)
{
	if (mWndMap.find(hwnd) != mWndMap.end())
	{
		return false;
	}
	mWndMap.insert(pair<HWND, XWindow*>(hwnd, win));
	return true;
}
bool WindowsManager::UnRegist(HWND hwnd)
{
	//TODO:需要加上线程同步
	std::map<HWND, XWindow*>::iterator iter = mWndMap.find(hwnd);
	if (iter != mWndMap.end())
	{
		mWndMap.erase(iter);
		return true;
	}
	return false;
}