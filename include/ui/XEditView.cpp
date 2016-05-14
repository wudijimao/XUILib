//
//  XEditView.cpp
//  XDUILib
//
//  Created by ximiao on 16/5/14.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "XEditView.hpp"

namespace  XUI {

	UIEditView::UIEditView() {
		mCursor.reset(new UIView());
		mCursor->setRect(XResource::XRectPro(10,0,2,20));
		mCursor->setBkgColor(XResource::XUIColor::blackColor());
		addSubView(mCursor);
	}

	void UIEditView::layoutSubViews() {
		auto rect = mCursor->getRect();
		rect.Height(this->getFixRect().Height());
		mCursor->setRect(rect);
	}

	void UIEditView::onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) {
		
	}

	void UIEditView::onMouseEvent(const std::vector<std::shared_ptr<XMouse>> &mouseEvent) {

	}

}
