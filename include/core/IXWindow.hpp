#pragma once
#include "stdafx.hpp"
#include "../res/XRect.hpp"
#include "IXCanvas.hpp"
#include "IXRender.hpp"
#include "input/XInputs.hpp"

namespace XUI {
    class UIViewController;
}
 

class SIMPLEDIRECTUI_API IXWindow
{
public:
    //fullScreen
    static std::shared_ptr<IXWindow> createWindow();
    
	virtual ~IXWindow(){};
    virtual void showInFront() = 0;
    virtual void setRootViewController(std::shared_ptr<XUI::UIViewController> rootViewController) = 0;
    //virtual std::shared_ptr<IXRender> render() = 0;
	//TODO::不应该有基于具体平台的
	//virtual HWND Hwnd() = 0;
	//virtual void NeedReDraw() = 0;
	//virtual HWND Create(HINSTANCE hInstance, int iCmdShow) = 0;
	//virtual void Drag(LPARAM lParam) = 0;
	//virtual const std::wstring& ID() = 0;
	//virtual void ID(const std::wstring& id) = 0;
    
    
	//virtual const XResource::XRect& Rect() const = 0;
	//virtual void Rect(const XResource::XRect& rect) = 0;
protected:
    std::shared_ptr<IXRender> _render;
};

