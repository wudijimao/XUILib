//
//  XError.hpp
//  XImage
//
//  Created by ximiao on 16/2/15.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#ifndef XError_hpp
#define XError_hpp

#include <stdio.h>
#include <string>
#include <map>

class XError {
    int code;
    std::string mes;
    void *userInfo; //notHold;
};



#endif /* XError_hpp */
