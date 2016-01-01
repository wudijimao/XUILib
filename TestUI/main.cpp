#include "../SimpleDirectUI/SimpleDirectUI.h"
#include "../SimpleDirectUI/XControls.h"

#include <windows.h>
#include <process.h>
#include <assert.h>
#include <string>
#include <vector>

#include "MainWnd.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	MainWnd *mainWnd = new MainWnd();
	SimpleDirectUI::getInstance()->Window().reset(mainWnd);
	mainWnd->InitControls();
	int ret = SimpleDirectUI::getInstance()->Show(hInstance, hPrevInstance, szCmdLine, iCmdShow);
	FreeConsole();
	return ret;
}