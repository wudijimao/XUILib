//
//  UIResponder.cpp
//  XDUILib
//
//  Created by ximiao on 16/5/30.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "UIResponder.hpp"

namespace XUI {
    UIResponder* UIResponder::sFirstResponder = nullptr;
    
    void UIResponder::becomFirstResponder(bool isFirst) {
        if (isFirst) {
            UIResponder::sFirstResponder = this;
        } else {
            UIResponder::sFirstResponder = nullptr;
        }
        
    }
    
    void UIResponder::insertText(const char *text) {
        
    }
    void UIResponder::deleteBackward() {
        
    }
    
}

