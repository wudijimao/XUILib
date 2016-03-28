#pragma once
#include "IBaseWindow.hpp"
#include "../res/XRectPro.hpp"

namespace XUI
{
	enum class MouseEventCommand
	{
		NONE = 0x00,
		CAPCURE_CATCH = 0x01,
		CAPCURE_REALEASE = 0x02
	};
    class UIViewController;
	class SIMPLEDIRECTUI_API UIView
	{
	public:
        friend class XWindow;
        friend class UIViewController;
        UIView();
        virtual ~UIView();
        virtual void setBkgColor(const std::shared_ptr<XResource::XUIColor> &color);
        virtual void setBkgImg(const std::shared_ptr<XResource::XImage> &img);
		
        virtual void drawRect(IXRender &render);
		
        virtual const XResource::XRectPro& getRect();
//		virtual void setRect(double x, double y, double width, double height) = 0;
//		virtual void setRect(const XResource::XRect& rect) = 0;
        virtual void setRect(const XResource::XRectPro& rect);
        
        //override
        virtual void layoutSubViews();
        
        //do not override these function below
        void setNeedReDraw();
        void setNeedLayout();
        std::shared_ptr<UIView> getSuperView();
        void addSubView(const std::shared_ptr<UIView> &view);
        const std::vector<std::shared_ptr<UIView>> subViews();
        //virtual const MouseStatusStruct& MouseStatus() = 0;
        
    public://protected:
        void layout(const XResource::XRect &absRect);
        void draw(IXRender &render);
        std::weak_ptr<UIView> _superView;
    private:
        std::vector<std::shared_ptr<UIView>> _subViews;
        XResource::XRectPro _layoutRect;
        XResource::XRect _rect;
        std::shared_ptr<XResource::XUIColor> _backGroundColor;
        bool _needReDraw = true;
        bool _needLayout = true;
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
//		virtual void Parent(IBaseControl* parent) = 0;
//		virtual void RefreshAbsRect() = 0;
//		virtual void AbsRect(const XResource::XRect& rect) = 0;
//		virtual XResource::XPoint GetMousePos() = 0;
	};
    
    
    
    class SIMPLEDIRECTUI_API UIViewController {
    public:
        const std::shared_ptr<UIView> getView();
        //override
        virtual void viewDidLoad();
    protected:
        friend class XWindow;
        void onSizeChange(XResource::XSize &size);
    private:
        void LoadView();
        bool _isLoaded = false;
        std::shared_ptr<UIView> _view;
    };
}






