//
//  UIView.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/27.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "UIView.hpp"
#include "GLRender.hpp"


namespace XUI
{
    UIView::UIView() {
        mRenderer = new GLRender();
        _backGroundColor = XResource::XUIColor::whiteColor();
    }
    UIView::~UIView() {
        delete mRenderer;
    }
    void UIView::setBkgColor(const std::shared_ptr<XResource::XUIColor> &color) {
        setNeedReDraw();
        _backGroundColor = color;
    }
    void UIView::setBkgImg(const std::shared_ptr<XResource::IXImage> &img) {
        setNeedReDraw();
        _backGroundImage = img;
    }
    
    void UIView::drawRect(IXRender &render) {
    }
    
    const XResource::XRectPro& UIView::getRect() {
        return _layoutRect;
    }
    //		virtual void setRect(double x, double y, double width, double height) = 0;
    //		virtual void setRect(const XResource::XRect& rect) = 0;
    void UIView::setRect(const XResource::XRectPro& rect) {
        _layoutRect = rect;
        setNeedLayout();
    }
    
    //override
    void UIView::layoutSubViews() {
        
    }
    
    //do not override these function below
    void UIView::setNeedReDraw() {
        _needReDraw = true;
        if(mBelongingViewController != nullptr) {
            mBelongingViewController->setNeedRedraw();
        }
    }
    void UIView::setNeedLayout() {
        _needLayout = true;
        _needReDraw = true;
    }
    std::shared_ptr<UIView> UIView::getSuperView() {
        return std::shared_ptr<UIView>();
    }
    void UIView::addSubView(const std::shared_ptr<UIView> &view) {
        //view->mRenderer->Init(render->)
        _subViews.push_back(view);
        view->_superView = this;
        if(mBelongingViewController != nullptr) {
            view->mBelongingViewController = mBelongingViewController;
            for (auto v : view->_subViews) {
                v->mBelongingViewController = mBelongingViewController;
            }
            mBelongingViewController->setNeedRedraw();
        }
    }
    const std::vector<std::shared_ptr<UIView>> UIView::subViews() {
        return _subViews;
    }
    void UIView::layout(const XResource::XRect &absRect) {
        if (_needLayout) {
            _needLayout = false;
            XResource::XRect tempRect = _rect;
            _rect = this->_layoutRect.MakeAbsRect(absRect);
            if (tempRect != _rect) {
                layoutSubViews();
                for (auto subView : _subViews) {
                    subView->layout(_rect);
                }
            }
        }
    }
    void UIView::draw() {
        if (_needReDraw) {
            _needReDraw = false;
            mRenderer->clear();
            mRenderer->setClipsToBounds(mIsClipsToBounds);
            if (mIsClipsToBounds) {
                mRenderer->setClipsBounds(_rect);
            }
            mRenderer->DrawBackGround(_backGroundColor->_color, _backGroundImage, _rect);
            drawRect(*mRenderer);
        }
        mRenderer->Submit();
        for (auto subView : _subViews) {
            subView->draw();
        }
    }
    
    void UIView::setClipsToBounds(bool clips) {
        if (mIsClipsToBounds != clips) {
            mIsClipsToBounds = clips;
            setNeedReDraw();
        }
    }
    bool UIView::isClipsToBounds() {
        return mIsClipsToBounds;
    }
    
    
    
    const std::shared_ptr<UIView> UIViewController::view() {
        if (!_isLoaded) {
            LoadView();
        }
        return _view;
    }
    //override
    void UIViewController::viewDidLoad() {
        
    }
    void UIViewController::onSizeChange(XResource::XSize &size) {
        XResource::XRectPro rect = _view->getRect();
        rect.Width(size.Width());
        rect.Height(size.Height());
        _view->setRect(rect);
    }
    
    void UIViewController::LoadView() {
        _isLoaded = true;
        _view = std::make_shared<XUI::UIView>();
        _view->mBelongingViewController = this;
        XResource::XRectPro rect;
        rect.setSize(mBelongWindow->size());
        rect.HAlign(XResource::XRectPro::HAlign_Stretch);
        rect.VAlign(XResource::XRectPro::VAlign_Stretch);
        _view->setRect(rect);
        
        viewDidLoad();
    }
    
    void UIViewController::presentViewControler(std::shared_ptr<UIViewController> controller, PresentAnimation ani) {
        this->mBelongWindow->setRootViewController(controller);
    }
    
}
