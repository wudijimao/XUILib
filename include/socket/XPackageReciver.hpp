//
//  XSocketBuffer.hpp
//  XDUILib
//
//  Created by ximiao on 16/6/23.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma once
#include <stdint.h>
#include "../core/stdafx.hpp"
#include "../res/XData.hpp"


class XPackageReciver {
public:
    bool put(const void *data, unsigned int size) {
        mBuf.appendData(data, size);
        while (true) {
            switch (status) {
                case 0: //get header
                    if (mBuf.size() > 2) {
                        mPackageLength = *((uint16_t*)mBuf.getBuf()) + 2;
                        status = 1;
                    } else {
                        return true;
                    }
                    break;
                case 1:
                    if (mBuf.size() >= mPackageLength) {
                        if(onRecive) {
                            onRecive(mBuf.getBufFrom(2), mPackageLength - 2);
                        }
                        mBuf.delBuf(0, mPackageLength);
                        status = 0;
                    } else {
                        return true;
                    }
                    break;
                default:
                    return false;
            }
        }
        return false;
    }
    std::function<void(const void *data, unsigned int size)> onRecive;
private:
    int status = 0;
    unsigned int mPackageLength;
    XResource::XData mBuf;
};