//
//  XView.cpp
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
    XView::XView() {
        mRenderer = new GLRender();
        _backGroundColor = XResource::XUIColor::whiteColor();
    }
    XView::~XView() {
        delete mRenderer;
    }
    void XView::setBkgColor(const std::shared_ptr<XResource::XUIColor> &color) {
        setNeedReDraw();
        _backGroundColor = color;
    }
    void XView::setBkgImg(const std::shared_ptr<XResource::IXImage> &img) {
        _backGroundImage = img;
        setNeedReDraw();
    }
    
    const std::shared_ptr<XResource::IXImage>& XView::bkgImage() {
        return _backGroundImage;
    }
    
    const std::shared_ptr<XResource::XStretchableImage>& XView::bkgStretchableImage() {
        return _backGroundStretchableImage;
    }
    
    void XView::setBkgImg(const std::shared_ptr<XResource::XStretchableImage> &img) {
        _backGroundStretchableImage = img;
        setNeedReDraw();
    }
    
    
    void XView::setMaskImg(const std::shared_ptr<XResource::IXImage> &img) {
        _maskImage = img;
        setNeedReDraw();
    }
    
    void XView::drawRect(IXRender &render) {
    }
    
    const XResource::XRectPro& XView::getRect() {
        return _layoutRect;
    }
    //		virtual void setRect(double x, double y, double width, double height) = 0;
    //		virtual void setRect(const XResource::XRect& rect) = 0;
    void XView::setRect(const XResource::XRectPro& rect) {
        _layoutRect = rect;
        setNeedLayout();
    }
    
    //override
    void XView::layoutSubViews() {
        
    }
    
    //do not override these function below
    void XView::setNeedReDraw() {
        _needReDraw = true;
        if(mBelongingViewController != nullptr) {
            mBelongingViewController->setNeedRedraw();
        }
    }
    void XView::setNeedLayout() {
		_needLayout = true;
		if (mBelongingViewController != nullptr) {
			mBelongingViewController->setNeedLayout();
		}
    }
    std::shared_ptr<XView> XView::getSuperView() {
        return std::shared_ptr<XView>();
    }
    void XView::addSubView(const std::shared_ptr<XView> &view) {
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
    bool XView::removeSubView(XView *view) {
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
    bool XView::removeFromSuperView() {
        return _superView->removeSubView(this);
    }
    
    
    const std::vector<std::shared_ptr<XView>> XView::subViews() {
        return _subViews;
    }
	void XView::layout(const XResource::XRect &absRect) {
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
    void XView::draw() {
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
    
    void XView::setClipsToBounds(bool clips) {
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
    
    bool XView::isClipsToBounds() {
        return mIsClipsToBounds;
    }
    
    void XView::makeClipsBounds() {
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
    void XView::setClipsToBoundsInternal() {
        mIsClipsToBoundsInternal = true;
        if (!_needLayout) {
            makeClipsBounds();
        }
        for (auto subView : _subViews) {
            subView->setClipsToBoundsInternal();
        }
    }
    void XView::clearClipsToBoundsInternal() {
        mIsClipsToBoundsInternal = mIsClipsToBounds;
        if (!mIsClipsToBoundsInternal) {
            for (auto subView : _subViews) {
                subView->clearClipsToBoundsInternal();
            }
        }
    }

	void XView::setVisible(bool visible) {
		if (visible != mIsVisable)
		{
			mIsVisable = visible;
			if (mBelongingViewController != nullptr) {
				mBelongingViewController->setNeedRedraw();
			}
		}
	}
    
	bool XView::isVisible() {
		return mIsVisable;
	}
    
	bool XView::hitTest(const std::shared_ptr<XInputWithPostion> &input) {
		if (_isInputEnable) {
			return _rect.isPointIn(input->mPosition);
		}
		else {
			return false;
		}
	}
    
    UIResponder* XView::nextResponder() {
        if (_superView != nullptr) {
            return _superView;
        } else {
            return mBelongingViewController;
        }
    }

    void XView::setTransform3D(const GLTransform3D &transform) {
        mTransform = transform;
    }
    const GLTransform3D& XView::getTransForm3D() {
        return mTransform;
    }
    const GLTransform3D& XView::getGloablTransForm3D() {
        return mCululatedGlobalTransform;
    }

}
