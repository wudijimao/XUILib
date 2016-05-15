//
//  XInput.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/30.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include "../stdafx.hpp"
#include "XInput.hpp"


enum class KeyEventType {
	UnSet,
	Down,
	Up,
};

enum class FunctionKeys {
	Contrl,
	Shift,
};

class SIMPLEDIRECTUI_API XKeyBoard : public XInput {
public:
};
