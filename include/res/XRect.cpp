#include "XRect.hpp"

namespace XResource {
    
    double gHighResolutionPixelScale = 1.0;
    
    XRect::XRect()
    {
    }
    
    XRect::XRect(double x, double y, double width, double height) :mPoint(x,y), mSize(width, height)
    {
        
    }
    XRect::XRect(const XDisplaySize&size) : mSize(size) {
        
    }
    
    XRect XRect::MakeAbsRect(const XRect& parentRect) const
    {
        XRect temp = *this;
        temp.moveX(parentRect.X());
        temp.moveY(parentRect.Y());
        return temp;
    }
    
    XRect::~XRect()
    {
    }
    
}