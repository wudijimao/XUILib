#pragma once
#include "ControlManager.hpp"
#include "IXRender.hpp"
#include "res/XRect.hpp"
#include "IBaseWindow.hpp"

class SIMPLEDIRECTUI_API XWindow : public IXWindow
{
public:
	XWindow(const XResource::XRect &rect);
	XWindow::~XWindow();
	///注意：窗口创建失败时会返回NULL
	HWND Create(HINSTANCE hInstance, int iCmdShow);
	XControls::ControlManager& Contrls();
	void OnDraw();
	//HWND Hwnd();
	void Drag(LPARAM lParam);
	virtual void NeedReDraw();
	virtual const MouseStatusStruct& MouseStauts();
	virtual const std::wstring& ID();
	virtual void ID(const std::wstring& id);
	virtual const XResource::XRect& Rect() const;
	virtual void Rect(const XResource::XRect& rect);
private:
protected:
	//一个窗口有且只有一个控件树
	XControls::ControlManager mCtrls;
	//还有一个绘图表面来绘制所有控件
	IXRender *mRender;

	std::wstring mAppNameClass;
	HINSTANCE mHInstance;
	HWND mHwnd;//窗体句柄
	XResource::XRect mRect;//窗体大小、位置
	MouseStatusStruct mMouseStatus;
	bool mNeedReDraw = true;
	std::wstring mID;
public:
	LRESULT RealWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
private:
	//不允许拷贝副本,防止意外拷贝出未在windowsManager里注册的窗体实例
	XWindow(const XWindow &){}//默认拷贝构造  
	XWindow & operator=(const XWindow &){ return *this; }//默认赋值运算符
};