#include "stdafx.h"
#include "SimpleDirectUI.h"
#include "XRect.h"
#include "XButton.h"
#include "XRichTextBox.h"
#include "XVScrollBar.h"
#include "XScrollViewer.h"
using namespace XResource;

int SimpleDirectUI::Show(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	mWindow->Create(hInstance, iCmdShow);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

boost::shared_ptr<SimpleDirectUI> SimpleDirectUI::getInstance()
{
	static boost::shared_ptr<SimpleDirectUI> ui(new SimpleDirectUI());
	return  ui;
}