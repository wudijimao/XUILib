#pragma once
#include "BaseWindow.h"
#include <map>
#include <windows.h>
#include <boost/signals2.hpp>

//不直接使用，创建窗口时会自动注册，销毁时自动反注册
//窗体销毁时会自动反注册，所以保存指针也是安全的
class WindowsManager
{
	std::map<HWND, XWindow*>mWndMap;
public:
	static WindowsManager& GetInstanc();
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);//接收窗体消息并分发到具体的window类实例上
public:
	bool Regist(HWND hwnd, XWindow* win);
	bool UnRegist(HWND hwnd);
private:
	//禁止外部创建、拷贝
	WindowsManager(){}
	WindowsManager(const WindowsManager &){}//默认拷贝构造  
	WindowsManager & operator=(const WindowsManager &){}//默认赋值运算符
};