#include "XButton.hpp"

namespace XUI {
    XButton::XButton(){
        mTextView.reset(new UITextView());
        mTextView->setBkgColor(XResource::XUIColor::clearColor());
        mTextView->setIsInputEnable(false);
        mTextView->setAlignmentH(UITextAlignmentH::Center);
        mTextView->setAlignmentV(UITextAlignmentV::Center);
        this->addSubView(mTextView);
        XButtonResHolder *res = &mResMap[(int)ButtonStates::NORMAL];
        res->mSetted = true;
        res->mFont = XResource::XFont::systemFont(14.0);
        res->mTextColor = XResource::XUIColor::blackColor();
        res->mText = "button";
        res->mBackGroundColor = XResource::XUIColor::grayColor();
        _setState(ButtonStates::NORMAL);
    };
    
    void XButton::onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) {
        for (auto t : touch) {
            switch(t->phase) {
                case TouchPhase::Began:
                    _down();
                    break;
                case TouchPhase::Ended:
                    if (onClick) {
                        onClick(*this);
                    }
                case TouchPhase::Cancelled:
                    _up();
                    break;
            }
        }
        XView::onTouch(touch);
    }
    
    void XButton::onMouseEvent(const std::vector<std::shared_ptr<XMouse>> &mouseEvents) {
		for (auto e : mouseEvents) {
			switch (e->eventType)
			{
			case MouseEventType::Down:
				_down();
				break;
			case MouseEventType::Up:
				if (onClick) {
					onClick(*this);
				}
				_up();
				break;
			default:
				break;
			}
		}
        XView::onMouseEvent(mouseEvents);
    }
    
    void XButton::select(bool bSel) {
        if (bSel != mIsSelected) {
            mIsSelected = bSel;
            _refreshStatus();
        }
    }
    bool XButton::isSelected() {
        return mIsSelected;
    }
    
    void XButton::setEnable(bool enable) {
        if (mIsEnable != enable) {
            mIsEnable = enable;
            _refreshStatus();
        }
    }
    bool XButton::isEnable() {
        return mIsEnable;
    }
    
    void XButton::_refreshStatus() {
        if (mIsEnable) {
            ButtonStates state = ButtonStates::NORMAL;
            if (mIsSelected) {
                state = ButtonStates::SELECTED;
            }
            if (mIsPushingDown) {
                state = (ButtonStates)((int)state + (int)ButtonStates::DOWN);
            } else if(mIsOvering) {
                state = (ButtonStates)((int)state + (int)ButtonStates::HOVER);
            }
            _setState(state);
        } else {
            _setState(ButtonStates::DISABLE);
        }
    }
    
    ButtonStates XButton::_getResExistState(ButtonStates state) {
        ButtonStates useResSettedStates = ButtonStates::NORMAL;
        switch (state) {
            case ButtonStates::SELECTED_DOWN:
            case ButtonStates::SELECTED_HOVER:
                if (mResMap[(int)state].mSetted) {
                    useResSettedStates = state;
                    break;
                }
            case ButtonStates::SELECTED:
                if (mResMap[(int)ButtonStates::SELECTED].mSetted) {
                    useResSettedStates = ButtonStates::SELECTED;
                }
                break;
            case ButtonStates::DISABLE:
            case ButtonStates::HOVER:
            case ButtonStates::DOWN:
                if (mResMap[(int)state].mSetted) {
                    useResSettedStates = state;
                }
                break;
            default:
                break;
        }
        return useResSettedStates;
    }
    
    //change button display in this function
    void XButton::_setState(ButtonStates state) {
        if (mState != state) {
            ButtonStates useResSettedStates = _getResExistState(state);
            if (mUsedResState != useResSettedStates) {
                mUsedResState = useResSettedStates;
                XButtonResHolder *res = &mResMap[(int)useResSettedStates];
                setBkgColor(res->mBackGroundColor);
                setBkgImg(res->mBackGroundImage);
                if (res->mAttrText) {
                    mTextView->setText(res->mAttrText);
                } else {
                    mTextView->setText(res->mText);
                    mTextView->setFont(res->mFont);
                    mTextView->setTextColor(res->mTextColor);
                }
            }
            mState = state;
            setNeedLayout();
        }
    }
    
    void XButton::_down() {
        mIsPushingDown = true;
        _refreshStatus();
    }
    void XButton::_up() {
        mIsPushingDown = false;
        _refreshStatus();
    }
    void XButton::_over() {
        if (!mIsOvering) {
            mIsOvering = true;
            _refreshStatus();
        }
    }
    void XButton::_out() {
        mIsOvering = false;
        _refreshStatus();
    }
    
    void XButton::layoutSubViews() {
        if(mIconView) {
            
        } else {
            mTextView->setRect(XResource::XRectPro(getRect().size()));
        }
    }
    
    void XButtonResHolder::copyFrom(const XButtonResHolder &res) {
        mBackGroundImage = res.mBackGroundImage;
        mImage = res.mImage;
        mBackGroundColor = res.mBackGroundColor;
        mTextColor = res.mTextColor;
        mFont = res.mFont;
        mText = res.mText;
        mAttrText = res.mAttrText;
    }
    
    XButtonResHolder * XButton::_getRes(ButtonStates state = ButtonStates::NORMAL) {
        XButtonResHolder *res = &mResMap[(int)state];
        ButtonStates useResSettedStates = _getResExistState(state);
        res->copyFrom(mResMap[(int)useResSettedStates]);
        res->mSetted = true;
        return res;
    }
    
    void XButton::setBackgroundImage(const std::shared_ptr<XResource::IXImage> &backgroundImage, ButtonStates state) {
        XButtonResHolder *res = _getRes(state);
        if (res->mBackGroundImage != backgroundImage) {
            res->mBackGroundImage = backgroundImage;
            if (state == mUsedResState) {
                setNeedReDraw();
            }
        }
    }
    
    void XButton::setIcon(const std::shared_ptr<XResource::IXImage> &image, ButtonStates state) {
        XButtonResHolder *res = _getRes(state);
        if (res->mImage != image) {
            res->mImage = image;
            if (state == mUsedResState) {
                setNeedReDraw();
            }
        }
    }
    void XButton::setBackgroundColor(const std::shared_ptr<XResource::XUIColor> &backgroundColor, ButtonStates state) {
        XButtonResHolder *res = _getRes(state);
        if (res->mBackGroundColor != backgroundColor) {
            res->mBackGroundColor = backgroundColor;
            if (state == mUsedResState) {
                setNeedReDraw();
            }
        }
    }
    void XButton::setTitleColor(const std::shared_ptr<XResource::XUIColor> &textColor, ButtonStates state) {
        XButtonResHolder *res = _getRes(state);
        if (res->mTextColor != textColor) {
            res->mTextColor = textColor;
            if (state == mUsedResState) {
                setNeedReDraw();
            }
        }
    }
    void XButton::setTitleFont(const std::shared_ptr<XResource::XFont> &font, ButtonStates state) {
        XButtonResHolder *res = _getRes(state);
        if (res->mFont != font) {
            res->mFont = font;
            if (state == mUsedResState) {
                setNeedLayout();
            }
        }
    }
    void XButton::setTitle(const XResource::XString &text, ButtonStates state) {
        XButtonResHolder *res = _getRes(state);
        res->mText = text;
        res->mAttrText = nullptr;
        if (state == mUsedResState) {
            mTextView->setText(res->mText);
            mTextView->setFont(res->mFont);
            mTextView->setTextColor(res->mTextColor);
            setNeedLayout();
        }
    }
    void XButton::setTitle(const std::shared_ptr<XResource::XAttributedString> &attrText, ButtonStates state) {
        XButtonResHolder *res = _getRes(state);
        if (res->mAttrText != attrText) {
            res->mAttrText = attrText;
            if (state == mUsedResState) {
                setNeedLayout();
            }
        }
    }
}



//void XButton::OnMouseDown(IBaseControl&)
//{
//	if (mState == ButtonStates::HOVER)
//	{
//		mState = ButtonStates::HOVERDOWN;
//	}
//	else
//	{
//		mState = ButtonStates::DOWN;
//	}
//	SetReDrawFlag();
//}
//void XButton::OnMouseUp(IBaseControl&)
//{
//	if (mState == ButtonStates::HOVER)
//	{
//		return;
//	}
//	if (mState == ButtonStates::HOVERDOWN)
//	{
//		mState = ButtonStates::HOVER;
//	}
//	else
//	{
//		mState = ButtonStates::NORMAL;
//	}
//	SetReDrawFlag();
//}
//void XButton::OnMouseEnter(IBaseControl&)
//{
//	mState = ButtonStates::HOVER;
//	SetReDrawFlag();
//}
//void XButton::OnMouseLeave(IBaseControl&)
//{
//	if (mState == ButtonStates::HOVERDOWN)
//	{
//		mState = ButtonStates::DOWN;
//	}
//	else
//	{
//		mState = ButtonStates::NORMAL;
//		SetReDrawFlag();
//	}
//}