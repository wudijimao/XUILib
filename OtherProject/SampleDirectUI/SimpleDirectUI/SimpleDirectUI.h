#pragma once
#include"DllExport.h"
#include "WindowsManager.h"
#include<Windows.h>



//实现功能之后写成接口 只暴露外边使用的部分
class SIMPLEDIRECTUI_API SimpleDirectUI {
public:
	static std::shared_ptr<SimpleDirectUI> getInstance();
	//exe实例句柄，是否显示窗口
	int Show(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		PSTR szCmdLine, int iCmdShow);
	std::shared_ptr<XWindow>&Window()
	{
		return mWindow;
	}
private:
	std::shared_ptr<XWindow>mWindow;
	SimpleDirectUI(void){};
};
