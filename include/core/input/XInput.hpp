//
//  XInput.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/30.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include "../stdafx.hpp"
#include "../../res/XResource.hpp"

class IXWindow;
enum class XInputType {
    Touch,
	Mouse,
};

class XInput {
public:
    virtual ~XInput(){};
    IXWindow *_belongWindow;
    virtual XInputType type() = 0;
};

namespace XUI {
	class UIView;
}

class XInputWithPostion : public XInput {
public:
	XResource::XPoint mPosition;
	XUI::UIView *_belongView;
};
