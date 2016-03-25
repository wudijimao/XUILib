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

	class SIMPLEDIRECTUI_API UIView
	{
	public:
		virtual ~UIView(){}
        virtual void setBkgColor(const std::shared_ptr<XResource::XUIColor> &color) {
            _backGroundColor = color;
        }
        virtual void setBkgImg(const std::shared_ptr<XResource::XImage> &img) {
            
        }
		
        virtual void draw(IXRender &render) {
            render.DrawBackGround(_backGroundColor->_color, _rect);
        }
		
        virtual XResource::XRectPro& getRect() {
            return _layoutRect;
        }
//		virtual void setRect(double x, double y, double width, double height) = 0;
//		virtual void setRect(const XResource::XRect& rect) = 0;
        virtual void setRect(const XResource::XRectPro& rect) {
            _layoutRect = rect;
        }
        
        virtual void layoutSubViews() {
            _rect.X(10);
            _rect.Y(10);
            _rect.Width(200);
            _rect.Height(200);
        }
        
        
        
        //do not override these function below
        void setNeedReDraw() {
            
        }
        void setNeedLayout() {
            
        }
        std::shared_ptr<UIView> getSuperView() {
            return std::shared_ptr<UIView>();
        }
        void addSubView(UIView* ctrl) {
            
        }
		//virtual const MouseStatusStruct& MouseStatus() = 0;
    private:
        std::weak_ptr<UIView> *_superView;
        XResource::XRectPro _layoutRect;
        XResource::XRect _rect;
        std::shared_ptr<XResource::XUIColor> _backGroundColor;
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
        UIView &getView() {
            if (!_isLoaded) {
                LoadView();
            }
            return _view;
        }
        virtual void viewDidLoad() {
            
        }
    private:
        void LoadView() {
            _isLoaded = true;
            XResource::XRectPro rect;
            rect.HAlign(XResource::XRectPro::HAlign_Stretch);
            rect.VAlign(XResource::XRectPro::VAlign_Stretch);
            _view.setRect(rect);
            viewDidLoad();
        }
        bool _isLoaded = false;
        UIView _view;
    };
}







