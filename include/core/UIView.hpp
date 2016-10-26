#pragma once

#include "UIResponder.hpp"
#include "../res/XRectPro.hpp"
#include "Animation/Animation.h"
#include "GL/GLTransform3D.hpp"

namespace XUI
{

    class SIMPLEDIRECTUI_API XView : public UIResponder, public IXRenderDataPovider {
	public:
        void setTransform3D(const GLTransform3D &transform);
        const GLTransform3D& getTransForm3D() const;
        friend class ::XWindow;
        friend class XUI::UIViewController;
        XView();
        virtual ~XView();
        virtual void setBkgColor(const std::shared_ptr<XResource::XUIColor> &color);
        virtual void setBkgImg(const std::shared_ptr<XResource::IXImage> &img);
        virtual void setBkgImg(const std::shared_ptr<XResource::XStretchableImage> &img);
        virtual void setMaskImg(const std::shared_ptr<XResource::IXImage> &img);
        
        const std::shared_ptr<XResource::IXImage>& bkgImage();
        const std::shared_ptr<XResource::XStretchableImage>& bkgStretchableImage();

        virtual const XResource::XRectPro& getRect();
//		virtual void setRect(double x, double y, double width, double height) = 0;
//		virtual void setRect(const XResource::XRect& rect) = 0;
        virtual void setRect(const XResource::XRectPro& rect);
        virtual void setTransformCenter(const XResource::XDisplayPoint &point) final;//Relative to self
        virtual const XResource::XDisplayPoint getTransformCenter() const final;
        //must override
        //can override(not have defalut behavior)
        virtual void layoutSubViews();
        virtual void drawRect(IXRender &render);
        //can override(have defalut behavior)
		virtual void setVisible(bool visible);
		virtual bool isVisible();
		virtual bool hitTest(const std::shared_ptr<XInputWithPostion> &input) override;
        //you had better not override these function below, unless you know what are you doing about
        virtual UIResponder* nextResponder() override;
        //do not override these function below
        void setClipsToBounds(bool clips);
        bool isClipsToBounds();
        void setNeedReDraw();
        void setNeedLayout();
		void addAnimation();
        std::shared_ptr<XView> getSuperView();
        void addSubView(const std::shared_ptr<XView> &view);
        bool removeSubView(XView *view);
        bool removeFromSuperView();
        
        const std::vector<std::shared_ptr<XView>> subViews();
        //virtual const MouseStatusStruct& MouseStatus() = 0;
        void setIsInputEnable(bool enable) {
            _isInputEnable = enable;
        }
        bool isInputEnable() {
            return _isInputEnable;
        }
        std::shared_ptr<XUI::XView> getResponseSubView(const std::shared_ptr<XInputWithPostion> &touch) {
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
		bool mIsVisable = true;
    private:
        void layout(const XResource::XRect &absRect);
        void draw();
        const GLTransform3D& getGloablTransForm3D();
    private:
        //internal
        void setClipsToBoundsInternal(const XView*view);
        void clearClipsToBoundsInternal(const XView*view);
        
        
        IXRender *mRenderer = nullptr;
        bool _isInputEnable = true;
        XView *_superView = nullptr;
        std::vector<std::shared_ptr<XView>> _subViews;
        XResource::XRectPro _layoutRect;
        XResource::XRect _rect;
        std::shared_ptr<XResource::XUIColor> _backGroundColor;
        std::shared_ptr<XResource::IXImage> _backGroundImage;
        std::shared_ptr<XResource::XStretchableImage> _backGroundStretchableImage;
        std::shared_ptr<XResource::IXImage> _maskImage;
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
    private:
        GLTransform3D mTransform;
        XResource::XDisplayPoint mTransformCenter;
        GLTransform3D mTransformCenterTransform3D;
        GLTransform3D mReltiveTransformFromTransformCenterToParent;
        GLTransform3D mCululatedGlobalTransform;
        static int sLayoutingTopLayerIndex;
        int mDrawLayerIndex;
        bool mIsClipsToBounds = false;
        //the parent drawLayerIndex which clipsToBounds is true
        XView *mClipsParentView = nullptr;
        
    public:
        virtual const GLTransform3D& rd_Transform() const override;
        
        virtual bool rd_NeedClipsChildren() const override;
        virtual int rd_DrawLayerIndex() const override;
        virtual int rd_BeClipsDrawLayerIndex() const override;
	};
    
}







