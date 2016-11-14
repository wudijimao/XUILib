//
//  ScrollView.cpp
//  XDUILib
//
//  Created by ximiao on 16/5/30.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "ScrollView.hpp"
#include "../core/UIViewController.hpp"

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
                this->moveTo(mTouchPos.X() - temp.X(), mTouchPos.Y() - temp.Y());
                mVelocityY = (mTouchPos.Y() - temp.Y())/ 3;
            }
                break;
            case TouchPhase::Ended: {
                //decelerate ani
                decelerateMove();
            }
            default:
                break;
        }
    }
    
    void ScrollView::moveTo(double x, double y) {
        if (mContentView) {
            auto rect = mContentView->getRect();
            rect.moveX(x);
            rect.moveY(y);
            mContentView->setRect(rect);
        }
    }

    void ScrollView::decelerateMove() {
        UIViewController *vc = getBelongingViewController();
        if (vc != nullptr) {
            double a = -0.6; //-2px/s
            if (mVelocityY < 0) {
                a = 0.6;
            }
            double vY1 = mVelocityY;
            XResource::XRectPro re = mContentView->getRect();
            auto ani = Animation::createAni([vY1, re, a, this](Animation*ani, unsigned long ms){
                double increacY = ((a * ani->getProcessedMs() * ani->getProcessedMs()) / 2) + vY1 * ani->getProcessedMs();
                XResource::XRectPro rect = re;
                rect.Y(rect.Y() + increacY);
                mContentView->setRect(rect);
            });
            ani->setDurationMS(vY1 / -a);
            vc->addAnimation(ani);
            ani->play();
        }
    }
    
}



