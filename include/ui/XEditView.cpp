//
//  XEditView.cpp
//  XDUILib
//
//  Created by ximiao on 16/5/14.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XEditView.hpp"
#include "../core/MutiThread/XTimer.hpp"
#include "../XDUILib.hpp"

namespace  XUI {

	UIEditView::UIEditView() {
		mCursor.reset(new XView());
		auto rect = XResource::XRectPro(10, 6, 0.5, 0);
		rect.Y2(6);
		rect.VAlign(XResource::XRectPro::VAlign_Stretch);
		mCursor->setRect(rect);

		mCursor->setBkgColor(XResource::XUIColor::blackColor());
		addSubView(mCursor);
		XDispatch::XTimer::timer(500)->setTickFun([this](const std::shared_ptr<XDispatch::XTimer>& t) {
			mCursor->setVisible(!mCursor->isVisible());
		}).start();
		becomFirstResponder();
	}

	void UIEditView::layoutSubViews() {
	}

	void UIEditView::onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) {
		XDUILib::XApp::thisApp().showSoftKeyBoard(true);
	}

	void UIEditView::onMouseEvent(const std::vector<std::shared_ptr<XMouse>> &mouseEvent) {

	}

	void UIEditView::drawRect(IXRender &render) {
		UITextView::drawRect(render);
	}

    void UIEditView::insertText(const char *text) {
        if (!mText) {
            mText = XResource::XAttributedString::attrStr("");
        }
        mText->appendString(text);
        this->setNeedReDraw();
    }
    
    void UIEditView::deleteBackward() {
        if (mText) {
            mText->removeStr(mText->length() - 1, 1);
            this->setNeedReDraw();
        }
    }
}
