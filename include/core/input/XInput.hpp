//
//  XInput.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/30.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include "../stdafx.hpp"

class IXWindow;
enum class XInputType {
    Touch,
};

class XInput {
public:
    virtual ~XInput(){};
    IXWindow *_belongWindow;
    virtual XInputType type() = 0;
    
};
