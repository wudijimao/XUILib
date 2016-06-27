//
//  XTextView.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/9.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include "../core/UIView.hpp"
#include "XTextView.hpp"

namespace XUI
{
    class SIMPLEDIRECTUI_API UIEditView : public UITextView {
	public:
		UIEditView();
		virtual void onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) override;
		virtual void onMouseEvent(const std::vector<std::shared_ptr<XMouse>> &mouseEvent) override;
		virtual void layoutSubViews() override;
        virtual void insertText(const char *text) override;
        virtual void deleteBackward() override;
	private:
		std::shared_ptr<UIView> mCursor;
    };
}
