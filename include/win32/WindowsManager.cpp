#include "WindowsManager.hpp"
using namespace std;

LRESULT CALLBACK WindowsManager::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//TODO:需要加上线程同步
	if (iMsg == WM_NCCREATE)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		XWindow_win* creatingWin = static_cast<XWindow_win*>(lpcs->lpCreateParams);
		
	}
	else
	{
		auto iter = GetInstanc().mWndMap.find(hwnd);
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
bool WindowsManager::Regist(HWND hwnd, XWindow_win* win)
{
	if (mWndMap.find(hwnd) != mWndMap.end())
	{
		return false;
	}
	mWndMap.insert(pair<HWND, XWindow_win*>(hwnd, win));
	return true;
}
bool WindowsManager::UnRegist(HWND hwnd)
{
	//TODO:需要加上线程同步
	auto iter = mWndMap.find(hwnd);
	if (iter != mWndMap.end())
	{
		mWndMap.erase(iter);
		return true;
	}
	return false;
}