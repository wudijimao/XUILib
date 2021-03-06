//
//  XTouch.hpp
//  XDUILib
//
//  Created by ximiao on 16/3/30.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "XInput.hpp"
#include "../../res/XResource.hpp"



enum class TouchPhase {
    Began,             // whenever a finger touches the surface.
    Moved,             // whenever a finger moves on the surface.
    Stationary,        // whenever a finger is touching the surface but hasn't moved since the previous event.
    Ended,             // whenever a finger leaves the surface.
    Cancelled,         // whenever a touch doesn't end but we need to stop tracking (e.g. putting device to face)
};

class SIMPLEDIRECTUI_API XTouch : public XInputWithPostion {
public:
    TouchPhase phase;
    unsigned int tapCount;
    virtual XInputType type() {
        return XInputType::Touch;
    }
};

