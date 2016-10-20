//
//  UIResponder.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/30.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma once
#include "IXWindow.hpp"


class XWindow;

namespace XUI
{
    
    enum class MouseEventCommand
    {
        NONE = 0x00,
        CAPCURE_CATCH = 0x01,
        CAPCURE_REALEASE = 0x02
    };
    class SIMPLEDIRECTUI_API UIResponder {
    public:
        virtual ~UIResponder(){}
        static UIResponder *sFirstResponder;
        //Lower Layer Interface
        virtual UIResponder* nextResponder() {
            return nullptr;
        }
        virtual void onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) {
            auto next = nextResponder();
            if (next != nullptr) {
                next->onTouch(touch);
            }
        }
        virtual void onMouseEvent(const std::vector<std::shared_ptr<XMouse>> &mouseEvent) {
        }
        virtual void onKeyEvent(const std::vector<std::shared_ptr<XKeyInput>> &keyEvent) {
        }
        virtual bool hitTest(const std::shared_ptr<XInputWithPostion> &touch) {
            return false;
        }
        virtual void becomFirstResponder(bool isFirst = true);
        virtual void insertText(const char *text);
        virtual void deleteBackward();
    };

}