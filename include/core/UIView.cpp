//
//  UIView.cpp
//  XDUILib
//
//  Created by ximiao on 16/3/27.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "UIView.hpp"
#include "GLRender.hpp"
#include "UIViewController.hpp"

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
        _backGroundImage = img;
        setNeedReDraw();
    }
    
    const std::shared_ptr<XResource::IXImage>& UIView::bkgImage() {
        return _backGroundImage;
    }
    
    const std::shared_ptr<XResource::XStretchableImage>& UIView::bkgStretchableImage() {
        return _backGroundStretchableImage;
    }
    
    void UIView::setBkgImg(const std::shared_ptr<XResource::XStretchableImage> &img) {
        _backGroundStretchableImage = img;
        setNeedReDraw();
    }
    
    
    void UIView::setMaskImg(const std::shared_ptr<XResource::IXImage> &img) {
        _maskImage = img;
        setNeedReDraw();
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
		if (mBelongingViewController != nullptr) {
			mBelongingViewController->setNeedLayout();
		}
    }
    std::shared_ptr<UIView> UIView::getSuperView() {
        return std::shared_ptr<UIView>();
    }
    void UIView::addSubView(const std::shared_ptr<UIView> &view) {
        //view->mRenderer->Init(render->)
        _subViews.push_back(view);
        view->_superView = this;
        if (mIsClipsToBoundsInternal) {
            view->setClipsToBoundsInternal();
        }
        if(mBelongingViewController != nullptr) {
            view->mBelongingViewController = mBelongingViewController;
            for (auto v : view->_subViews) {
                v->mBelongingViewController = mBelongingViewController;
            }
            mBelongingViewController->setNeedRedraw();
        }
    }
    bool UIView::removeSubView(UIView *view) {
        auto iter = _subViews.begin();
        auto end = _subViews.end();
        while (iter != end) {
            if ((*iter).get() == view) {
                _subViews.erase(iter);
                view->_superView = nullptr;
                setNeedReDraw();
                return true;
            }
            ++iter;
        }
        return false;
    }
    bool UIView::removeFromSuperView() {
        return _superView->removeSubView(this);
    }
    
    
    const std::vector<std::shared_ptr<UIView>> UIView::subViews() {
        return _subViews;
    }
	void UIView::layout(const XResource::XRect &absRect) {
		XResource::XRect tempRect = _rect;
		_rect = this->_layoutRect.MakeAbsRect(absRect);
		bool posChanged = (tempRect.point() != _rect.point());
		bool sizeChanged = (tempRect.size() != _rect.size());
		if (sizeChanged) {
			setNeedReDraw();
			if (_needLayout && sizeChanged) {
				layoutSubViews();
				for (auto subView : _subViews) {
					subView->setNeedLayout();
				}
			}
        } else if(posChanged) {
            mRenderer->move(_rect.point() - tempRect.point());
            if(mBelongingViewController != nullptr) {
                mBelongingViewController->setNeedRedraw();
            }
        }
        if (_needLayout && mIsClipsToBoundsInternal) {
            makeClipsBounds();
        }
        _needLayout = false;
		for (auto subView : _subViews) {
			subView->layout(_rect);
		}
	}
    void UIView::draw() {
		if (!mIsVisable)
		{
			return;
		}
        if (_needReDraw) {
            _needReDraw = false;
            mRenderer->clear();
            mRenderer->setClipsToBounds(mIsClipsToBoundsInternal);
            mRenderer->setMask(_maskImage);
            if (mIsClipsToBoundsInternal) {
                mRenderer->setBounds(mClipsBounds);
            }
            mRenderer->DrawBackGround(_backGroundColor->_color, _backGroundImage, _rect);
            if(_backGroundStretchableImage) {
                mRenderer->DrawImage(_backGroundStretchableImage, _rect);
            }
            drawRect(*mRenderer);
        }
        mRenderer->Submit();
        for (auto subView : _subViews) {
            subView->draw();
        }
    }
    
    void UIView::setClipsToBounds(bool clips) {
        if (mIsClipsToBounds != clips) {
            mIsClipsToBoundsInternal = mIsClipsToBounds = clips;
            if (clips) {
                mClipsBounds = _rect;
                for (auto subView : _subViews) {
                    subView->setClipsToBoundsInternal();
                }
            } else {
                mIsClipsToBoundsInternal = _superView->mIsClipsToBoundsInternal;
                if (!mIsClipsToBoundsInternal) {
                    for (auto subView : _subViews) {
                        clearClipsToBoundsInternal();
                    }
                }
            }
            setNeedReDraw();
        }
    }
    
    bool UIView::isClipsToBounds() {
        return mIsClipsToBounds;
    }
    
    void UIView::makeClipsBounds() {
        if (_superView->mIsClipsToBoundsInternal) {
            if (mIsClipsToBounds) {
                mClipsBounds.X((std::max)(_rect.X(), _superView->mClipsBounds.X()));
                mClipsBounds.Y(std::max<double>(_rect.Y(), _superView->mClipsBounds.Y()));
                mClipsBounds.Width(std::min<double>(_rect.rX(), _superView->mClipsBounds.rX()) - mClipsBounds.X());
                mClipsBounds.Height(std::min<double>(_rect.bY(), _superView->mClipsBounds.bY()) - mClipsBounds.Y());
            } else {
                mClipsBounds = _superView->mClipsBounds;
            }
        } else if(mIsClipsToBounds) {
            mClipsBounds = _rect;
        }
    }
    void UIView::setClipsToBoundsInternal() {
        mIsClipsToBoundsInternal = true;
        if (!_needLayout) {
            makeClipsBounds();
        }
        for (auto subView : _subViews) {
            subView->setClipsToBoundsInternal();
        }
    }
    void UIView::clearClipsToBoundsInternal() {
        mIsClipsToBoundsInternal = mIsClipsToBounds;
        if (!mIsClipsToBoundsInternal) {
            for (auto subView : _subViews) {
                subView->clearClipsToBoundsInternal();
            }
        }
    }
    
    

	void UIView::setVisible(bool visible) {
		if (visible != mIsVisable)
		{
			mIsVisable = visible;
			if (mBelongingViewController != nullptr) {
				mBelongingViewController->setNeedRedraw();
			}
		}
	}
    
	bool UIView::isVisible() {
		return mIsVisable;
	}
    
	bool UIView::hitTest(const std::shared_ptr<XInputWithPostion> &input) {
		if (_isInputEnable) {
			return _rect.isPointIn(input->mPosition);
		}
		else {
			return false;
		}
	}
    
    UIResponder* UIView::nextResponder() {
        if (_superView != nullptr) {
            return _superView;
        } else {
            return mBelongingViewController;
        }
    }
    
}
