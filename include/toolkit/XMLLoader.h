/********************************************************************
*   Copyright(C),2009-2014,XStudio
*	All rights reserved.
*	created:	2014/12/21
*	author:		wudijimao
*   e-mail:		wudijimao@qq.com
*	version:    1.0
*	purpose:	运行时载入XML界面的类
*********************************************************************/
#pragma once
#include "BaseWindow.h"
#include <string>
#include <stdio.h>


class SIMPLEDIRECTUI_API XMLLoader
{
public:
	bool Load(const char* fileName);
	//先这么放着，后续需要设计一下接口
	XWindow *mWindow;
private:
	void 分析();
};
