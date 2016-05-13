#pragma once
#include "IXWindow.hpp"
#include "../res/XRectPro.hpp"

class XWindow;

namespace XUI
{
    
	enum class MouseEventCommand
	{
		NONE = 0x00,
		CAPCURE_CATCH = 0x01,
		CAPCURE_REALEASE = 0x02
	};
    class SIMPLEDIRECTUI_API UIResponder {
    public:
        virtual void onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) {
        }
		virtual void onMouseEvent(const std::vector<std::shared_ptr<XMouse>> &mouseEvent) {
		}
        virtual bool hitTest(const std::shared_ptr<XInputWithPostion> &touch) {
            return false;
        }
    };
    
    class UIViewController;
    class SIMPLEDIRECTUI_API UIView : public UIResponder
	{
	public:
        friend class ::XWindow;
        friend class UIViewController;
        UIView();
        virtual ~UIView();
        virtual void setBkgColor(const std::shared_ptr<XResource::XUIColor> &color);
        virtual void setBkgImg(const std::shared_ptr<XResource::IXImage> &img);
		
		
        virtual const XResource::XRectPro& getRect();
//		virtual void setRect(double x, double y, double width, double height) = 0;
//		virtual void setRect(const XResource::XRect& rect) = 0;
        virtual void setRect(const XResource::XRectPro& rect);
        //must override
        //override(not have defalut behavior)
        virtual void layoutSubViews();
        virtual void drawRect(IXRender &render);
        //can override(have defalut behavior)
        virtual bool hitTest(const std::shared_ptr<XInputWithPostion> &input) {
            if (_isInputEnable) {
                return _rect.isPointIn(input->mPosition);
            } else {
                return false;
            }
        }
        //do not override these function below
        void setClipsToBounds(bool clips);
        bool isClipsToBounds();
        void setNeedReDraw();
        void setNeedLayout();
        std::shared_ptr<UIView> getSuperView();
        void addSubView(const std::shared_ptr<UIView> &view);
        const std::vector<std::shared_ptr<UIView>> subViews();
        //virtual const MouseStatusStruct& MouseStatus() = 0;
        void setIsInputEnable(bool enable) {
            _isInputEnable = enable;
        }
        bool isInputEnable() {
            return _isInputEnable;
        }
        std::shared_ptr<XUI::UIView> getResponseSubView(const std::shared_ptr<XInputWithPostion> &touch) {
            auto rIter =  _subViews.rbegin();
            auto rEnd = _subViews.rend();
            while (rIter != rEnd) {
                if ((*rIter)->hitTest(touch)) {
                    auto view = (*rIter)->getResponseSubView(touch);
                    if (view == nullptr) {
                        return *rIter;
                    } else {
                        return view;
                    }
                };
                ++rIter;
            }
            return nullptr;
        }
    protected:
        const XResource::XRect &getFixRect() {
            return _rect;
        }
    private:
        void layout(const XResource::XRect &absRect);
        void draw();
    private:
        bool mIsClipsToBounds = false;
        IXRender *mRenderer;
        bool _isInputEnable = true;
        UIView *_superView;
        std::vector<std::shared_ptr<UIView>> _subViews;
        XResource::XRectPro _layoutRect;
        XResource::XRect _rect;
        std::shared_ptr<XResource::XUIColor> _backGroundColor;
        std::shared_ptr<XResource::IXImage> _backGroundImage;
        bool _needReDraw = true;
        bool _needLayout = true;
        UIViewController *mBelongingViewController = nullptr;
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
    
    
    
    class SIMPLEDIRECTUI_API UIViewController : public UIResponder {
    public:
        friend class ::XWindow;
        //override
        virtual void viewDidLoad();
        //do not override
		const std::shared_ptr<UIView> view();
        void setNeedRedraw() {
            if (mBelongWindow != nullptr) {
                mBelongWindow->setNeedReDraw();
            }
        }
    protected:
        void onSizeChange(XResource::XSize &size);
        IXWindow *mBelongWindow = nullptr;
    private:
        
        void LoadView();
        bool _isLoaded = false;
        std::shared_ptr<UIView> _view;
    };
}







