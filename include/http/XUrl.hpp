//
//  XUrl.hpp
//  XDUILib
//
//  Created by ximiao on 16/6/15.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma once

#include "../core/stdafx.hpp"
#include <assert.h>

class SIMPLEDIRECTUI_API XUrl {
private:
    static unsigned char ToHex(unsigned char x);
    static unsigned char FromHex(unsigned char x);
public:
    static std::string UrlEncode(const std::string& str);
    static std::string UrlDecode(const std::string& str);
};


