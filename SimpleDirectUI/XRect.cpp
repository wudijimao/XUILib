#include "stdafx.h"
#include "XRect.h"

using namespace XResource;
XRect::XRect()
{
}

XResource::XRect::XRect(double x, double y, double width, double height) :mX(x), mY(y), mWidth(width), mHeight(height)
{

}

XRect XResource::XRect::MakeAbsRect(const XRect& parentRect) const
{
	XRect temp = *this;
	temp.X(parentRect.X() + mX);
	temp.Y(parentRect.Y() + mY);
	return temp;
}

XRect::~XRect()
{
}