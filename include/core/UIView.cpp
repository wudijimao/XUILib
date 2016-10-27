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
    int XView::sLayoutingTopLayerIndex = 0;
    
    XView::XView() {
        mRenderer = new GLRender(this);
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
    void XView::setTransformCenter(const XResource::XDisplayPoint &point) {
        mTransformCenter = point;
        mTransformCenterTransform3D.setPosition(-point.X(), -point.Y());
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
        if (mIsClipsToBounds) {
            view->setClipsToBoundsInternal(this);
        } else if(mClipsParentView != nullptr) {
            view->setClipsToBoundsInternal(mClipsParentView);
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
                view->clearClipsToBoundsInternal(this);
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
		this->_layoutRect.makeRealativeAbsRect(absRect.size(), _rect);
        
        mReltiveTransformFromTransformCenterToParent.setPosition(_rect.X() + mTransformCenter.X(), _rect.Y() + mTransformCenter.Y());
        
        
        //可以判断 一定情况下只更新transform
        if (_superView != nullptr) {
            mCululatedGlobalTransform = mTransformCenterTransform3D * mTransform * mReltiveTransformFromTransformCenterToParent  * _superView->getGloablTransForm3D();
        } else {
            sLayoutingTopLayerIndex = 0;
            mCululatedGlobalTransform = mTransformCenterTransform3D * mTransform * mReltiveTransformFromTransformCenterToParent;
            
        }
        if (mIsClipsToBounds) {
            ++sLayoutingTopLayerIndex;
        }
        mClipsLayerIndex = sLayoutingTopLayerIndex;
        
		bool sizeChanged = (tempRect.size() != _rect.size());
		if (sizeChanged) {
			setNeedReDraw();
			if (_needLayout && sizeChanged) {
				layoutSubViews();
				for (auto subView : _subViews) {
					subView->setNeedLayout();
				}
			}
        }
        _needLayout = false;
		for (auto subView : _subViews) {
			subView->layout(_rect);
		}
	}
    void XView::draw() {
        //transfrom 会将坐标系转换成本地坐标系，绘制时以本View左上角为0,0
		if (!mIsVisable)
		{
			return;
		}
        if (_needReDraw) {
            _needReDraw = false;
            mRenderer->clear();
            mRenderer->setMask(_maskImage);
            XResource::XRect rect(_rect.size());
            mRenderer->DrawBackGround(_backGroundColor->_color, _backGroundImage, rect, mIsClipsToBounds);
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
            mIsClipsToBounds = clips;
            if (clips) {
                for (auto subView : _subViews) {
                    subView->setClipsToBoundsInternal(this);
                }
            } else {
                for (auto subView : _subViews) {
                    clearClipsToBoundsInternal(this);
                }
            }
            setNeedReDraw();
        }
    }
    
    bool XView::isClipsToBounds() {
        return mIsClipsToBounds;
    }
    
    void XView::setClipsToBoundsInternal(const XView*view) {
        if (mClipsParentView == nullptr) {
            mClipsParentView = view;
            for (auto subView : _subViews) {
                subView->setClipsToBoundsInternal(view);
            }
        }
    }
    void XView::clearClipsToBoundsInternal(const XView *view) {
        if (mClipsParentView == view) {
            mClipsParentView = view->mClipsParentView;
            for (auto subView : _subViews) {
                subView->clearClipsToBoundsInternal(view);
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
    const GLTransform3D& XView::getTransForm3D() const {
        return mTransform;
    }
    const GLTransform3D& XView::getGloablTransForm3D() {
        return mCululatedGlobalTransform;
    }
    
    const XResource::XDisplayPoint XView::getTransformCenter() const {
        return mTransformCenter;
    }

    
    const GLTransform3D& XView::rd_Transform() const {
        return this->mCululatedGlobalTransform;
    }
    
    bool XView::rd_NeedClipsChildren() const {
        return mIsClipsToBounds;
    }
    int XView::rd_ClipsLayerIndex() const {
        return mClipsLayerIndex;
    }
    int XView::rd_BeClipsDrawLayerIndex(bool isClipsChildren) const {
        if (mIsClipsToBounds && !isClipsChildren) {
            return mClipsLayerIndex;
        } else if (mClipsParentView == nullptr) {
            return 0;
        } else {
            return mClipsParentView->mClipsLayerIndex;
        }
    }
}
