//
//  XRange.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/11.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once

#include "../core/stdafx.hpp"

namespace XResource
{
    
    struct XRange {
        XRange(){}
        XRange(unsigned long in_location, unsigned long in_length):location(in_location), length(in_length){}
        inline unsigned long rightPosition() const {
            return location + length;
        }
        unsigned long location = ULONG_MAX;
        unsigned long length = 0;
    };
    
}