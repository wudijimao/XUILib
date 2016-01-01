#pragma once
#include "../SimpleDirectUI/XControls.h"
#include "../SimpleDirectUI/SimpleDirectUI.h"
#include <iostream>


class MainWnd : public XWindow
{
public:
	MainWnd();
	~MainWnd();
	void InitControls();
};