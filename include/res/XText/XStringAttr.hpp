//
//  XStringAttr.hpp
//  XDUILib
//
//  Created by ximiao on 16/5/11.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#include "../../core/stdafx.hpp"
#include "../XRange.hpp"

namespace XResource
{
    enum XAttrStrKeyEnum {
        XAttrStrKey_Font = 0,
        XAttrStrKey_Color = 1,
        XAttrStrKey_Custom = 2000,
        //canAddToBelow or use XAttrStrKey_Customn + 1(2,3,4...)
    };
    
    class XStringAttr {
    public:
        virtual ~XStringAttr(){};
        virtual XAttrStrKeyEnum getKey() = 0;
        //for debug
        const char *getKeyName() {
            XAttrStrKeyEnum key = getKey();
            switch (key) {
                case XAttrStrKey_Font:
                    return "XAttrStrKey_Font";
                case XAttrStrKey_Color:
                    return "XAttrStrKey_Color";
                default:
                    break;
            }
            return "XAttrStrKey_Custom";
        }
    };
    
    typedef std::shared_ptr<XStringAttr> XStrAttrPtr;
    
    struct XStrAttrContainer {
        XStrAttrPtr attr;
        XRange range;
        XStrAttrContainer(){}
        XStrAttrContainer(const XStrAttrPtr &in_attr, const XRange &in_range) : range(in_range), attr(in_attr) {}
    };
    
    typedef std::vector<XStrAttrContainer> XStrAttrVec;
    
}