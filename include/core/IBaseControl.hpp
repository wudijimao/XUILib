#pragma once
#include "GDIRender.hpp"
#include "IBaseWindow.hpp"
#include "res\XRectPro.hpp"
namespace XControls
{
	enum class MouseEventCommand
	{
		NONE = 0x00,
		CAPCURE_CATCH = 0x01,
		CAPCURE_REALEASE = 0x02
	};

	class SIMPLEDIRECTUI_API IBaseControl
	{
		friend class BaseControl;
	public:
		virtual ~IBaseControl(){}
		virtual IXWindow& BelongWnd() = 0;
		virtual bool BelongWnd(IXWindow *wnd) = 0;
		virtual void BkgColor(byte a, byte r, byte b, byte g) = 0;
		virtual void BkgImg(const std::wstring& path) = 0;
		virtual void AddChild(IBaseControl* ctrl) = 0;
		virtual void Draw(IXRender &g) = 0;
		virtual bool NeedReDraw() = 0;

		virtual const XResource::XRectPro& Rect() const = 0;
		virtual XResource::XRectPro& Rect() = 0;
		virtual void Rect(double x, double y, double width, double height) = 0;
		virtual void Rect(const XResource::XRect& rect) = 0;
		virtual void Rect(const XResource::XRectPro& rect) = 0;
		virtual const XResource::XRect& AbsRect() = 0;

		virtual void MouseEvent(UINT key, int x, int y) = 0;
		virtual void KeyBoardEvent(UINT key, WPARAM wParam, LPARAM lParam) = 0;
		virtual void DragWindowEnable(bool) = 0;

		virtual IBaseControl& Parent() = 0;

		virtual const MouseStatusStruct& MouseStatus() = 0;
	public:
		//鼠标事件
		/*boost::signals2::signal<void(IBaseControl&)> MouseDown;
		boost::signals2::signal<void(IBaseControl&)> MouseUp;
		boost::signals2::signal<void(IBaseControl&)> MouseClick;
		boost::signals2::signal<void(IBaseControl&)> MouseEnter;
		boost::signals2::signal<void(IBaseControl&)> MouseWheel;
		boost::signals2::signal<void(IBaseControl&)> MouseLeave;
		boost::signals2::signal<void(IBaseControl&)> MouseMove;*/
	public:
		//键盘事件
		//boost::signals2::signal<void(IBaseControl&, WPARAM, LPARAM)> KeyDown;
	public:
		//其他事件
		/*
		boost::signals2::signal<void(IBaseControl&)> GotFocus;
		boost::signals2::signal<void(IBaseControl&)> LostFocus;
		//第二个参数是将要设置成的，从控件里可以获取到之前的size
		boost::signals2::signal<void(IBaseControl&, XResource::XRect&)> SizeChange;
		boost::signals2::signal<void(IBaseControl&, XResource::XRect&)> PosChange;*/
	protected:
		virtual void Parent(IBaseControl* parent) = 0;
		virtual void RefreshAbsRect() = 0;
		virtual void AbsRect(const XResource::XRect& rect) = 0;
		virtual XResource::XPoint GetMousePos() = 0;
	};
	typedef std::shared_ptr<IBaseControl> BaseControlPtr;

}