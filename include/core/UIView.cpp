//
//  UIView.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/27.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "UIView.hpp"


namespace XUI
{
    UIView::UIView() {
        _backGroundColor = XResource::XUIColor::whiteColor();
    }
    UIView::~UIView(){}
    void UIView::setBkgColor(const std::shared_ptr<XResource::XUIColor> &color) {
        _backGroundColor = color;
    }
    void UIView::setBkgImg(const std::shared_ptr<XResource::XImage> &img) {
        
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
        setNeedReDraw();
    }
    
    //override
    void UIView::layoutSubViews() {
        
    }
    
    //do not override these function below
    void UIView::setNeedReDraw() {
        _needReDraw = true;
    }
    void UIView::setNeedLayout() {
        _needLayout = true;
    }
    std::shared_ptr<UIView> UIView::getSuperView() {
        return std::shared_ptr<UIView>();
    }
    void UIView::addSubView(const std::shared_ptr<UIView> &view) {
        _subViews.push_back(view);
    }
    const std::vector<std::shared_ptr<UIView>> UIView::subViews() {
        return _subViews;
    }
    void UIView::layout(const XResource::XRect &absRect) {
        if (_needLayout) {
            _needLayout = false;
            _rect = this->_layoutRect.MakeAbsRect(absRect);
            layoutSubViews();
            for (auto subView : _subViews) {
                subView->layout(_rect);
            }
        }
    }
    void UIView::draw(IXRender &render) {
        render.DrawBackGround(_backGroundColor->_color, _rect);
        drawRect(render);
        for (auto subView : _subViews) {
            subView->draw(render);
        }
    }
    
    
    const std::shared_ptr<UIView> UIViewController::getView() {
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
        XResource::XRectPro rect;
        rect.HAlign(XResource::XRectPro::HAlign_Stretch);
        rect.VAlign(XResource::XRectPro::VAlign_Stretch);
        _view->setRect(rect);
        viewDidLoad();
    }
}
