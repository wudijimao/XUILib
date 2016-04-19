#include "MainWnd.h"



using namespace XResource;
MainWnd::MainWnd() : XWindow(XResource::XRect(0, 0, 800, 630))
{
}

MainWnd::~MainWnd()
{
}

void MainWnd::InitControls()
{
	//先暂时在这里向窗体添加控件,大概应该单独写函数，实现支持XML读取，及写程序订制两种功能。XML读取也是调用写程序订制的功能实现。
	Contrls().GetRoot()->BkgImg(L"../Res/bkg.png");
	XControls::BaseControlPtr baseControl = Contrls().GetRoot();
	//((XControls::BaseControl*)(baseControl.get()))->DragWindowEnable(true);

	//标题栏
	XControls::BaseControl* contrl = new XControls::BaseControl(L"captain");
	contrl->BkgImg(L"../Res/title.png");
	contrl->Rect(0, 0, 800, 24);
	contrl->DragWindowEnable(true);
	Contrls().GetRoot()->AddChild(contrl);
	//contrl->KeyDown.connect(OnKeyDown);

	contrl = new XControls::BaseControl(L"page_panel");
	contrl->Rect(0, 24, 0, 0);
	contrl->Rect().Y2(30.0);
	contrl->Rect().HAlign(XRectPro::HAlign_Stretch);
	contrl->Rect().VAlign(XRectPro::VAlign_Stretch);
	Contrls().GetRoot()->AddChild(contrl);
	XControls::BaseControl* text = new XControls::XTextBox(L"text_box");
	text->Rect(100, 24, 100, 20);
	text->BkgColor(255, 200, 100, 100);
	contrl->AddChild(text);

	contrl = new XControls::BaseControl(L"bottom_list");
	contrl->Rect(0, 0, 0, 30);
	contrl->Rect().HAlign(XRectPro::HAlign_Stretch);
	contrl->Rect().VAlign(XRectPro::VAlign_Bottom);
	contrl->BkgColor(255, 100, 100, 100);
	Contrls().GetRoot()->AddChild(contrl);

	auto *btn = new XControls::XButton(L"btn");
	btn->Rect(0, 20, 50, 50);
	btn->BkgColor(255, 255, 0, 0);
	Contrls().GetRoot()->AddChild(btn); 



	
}

