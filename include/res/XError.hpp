//
//  XError.hpp
//  XImage
//
//  Created by ximiao on 16/2/15.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once

#include <stdio.h>
#include <string>
#include <map>


namespace XResource
{
    
    class XError {
        int code;
        std::string mes;
        void *userInfo; //notHold;
    };
    
}

