//
//  XImage.hpp
//  XImage
//
//  Created by ximiao on 16/2/15.
//  Copyright © 2016年 ximiao. All rights reserved.
//

#ifndef XImage_hpp
#define XImage_hpp

#include <stdio.h>
#include "XData.hpp"
#include <vector>
#include "IXImage.hpp"
#include "XImageDecoder.hpp"


class XImage : public IXImage {
public:
    XImageDecoder *mDecoder;
    XImage () {
    }
    XImage (const char *filePath);
    ~XImage();
    virtual int width() override;
    virtual int height() override;
    virtual int count() override;
    virtual bool getImage(void *outBuf, int index) override;
private:
    static std::vector<XImageDecoder*> *decoders;
};


#endif /* XImage_hpp */
