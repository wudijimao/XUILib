#include "../SimpleDirectUI/XMLManager.h"
#include "../SimpleDirectUI/XMLLoader.h"
#include "../SimpleDirectUI/SimpleDirectUI.h"
#include <windows.h>



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	XMLLoader loader;
	loader.Load("XMLFile.xml");
	SimpleDirectUI::getInstance()->Window().reset(loader.mWindow);
	SimpleDirectUI::getInstance()->Show(hInstance, iCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	FreeConsole();
	return msg.wParam;
}
