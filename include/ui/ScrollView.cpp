//
//  ScrollView.cpp
//  XDUILib
//
//  Created by ximiao on 16/5/30.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "ScrollView.hpp"

namespace XUI {
    
    ScrollView::ScrollView() {
        setClipsToBounds(true);
        setIsInputEnable(true);
    }
    
    void ScrollView::onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) {
        switch (touch.front()->phase) {
            case TouchPhase::Began:
                mTouchPos = touch.front()->mPosition;
                break;
            case TouchPhase::Moved: {
                auto temp = mTouchPos;
                mTouchPos = touch.front()->mPosition;
                this->movePos(mTouchPos.X() - temp.X(), mTouchPos.Y() - temp.Y());
            }
                break;
            default:
                break;
        }
    }
    
    void ScrollView::movePos(double x, double y) {
        if (mContentView) {
            auto rect = mContentView->getRect();
            rect.moveX(x);
            rect.moveY(y);
            mContentView->setRect(rect);
        }
    }
    
}

