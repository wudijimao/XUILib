#pragma once
#include "XRect.hpp"

namespace XResource
{
	class XRectPro : public XRect
	{
	public:
		XRectPro(){}
        XRectPro(double x, double y, double width, double height) : XResource::XRect(x,y,width,height) {}
		XRectPro(const XRect& rect)
		{
            mSize = rect.size();
            mPoint = rect.point();
		}
		double X2() const
		{
			return mX2;
		}
		void X2(double marginRight)
		{
			mX2 = marginRight;
		}
		double Y2() const
		{
			return mY2;
		}
		void Y2(double marginBottom)
		{
			mY2 = marginBottom;
		}
		enum HAlignType
		{
			HAlign_Left = 0,
			HAlign_Center,
			HAlign_Right,
			HAlign_Stretch
		};
		enum VAlignType
		{
			VAlign_Top = 0,
			VAlign_Center,
			VAlign_Bottom,
			VAlign_Stretch
		};
		HAlignType HAlign() const
		{
			return mHAlign;
		}
		void HAlign(HAlignType hAlign)
		{
			mHAlign = hAlign;
		}
		VAlignType VAlign() const
		{
			return mVAlign;
		}
		void VAlign(VAlignType vAlign)
		{
			mVAlign = vAlign;
		}
		virtual XRect MakeAbsRect(const XRect& parentAbsRect) const
		{
			XRect temp;
			switch (mHAlign)
			{
			case XResource::XRectPro::HAlign_Left:
				temp.Width(Width());
				temp.X(parentAbsRect.X() + X());
				break;
			case XResource::XRectPro::HAlign_Center:
			{
				temp.Width(Width());
				//中心点
				double center = parentAbsRect.X() + parentAbsRect.Width() / 2.0;
				//中心点根据X X2 偏移
				center += (X() - mX2);
				temp.X(center - Width() / 2.0);
				break;
			}	
			case XResource::XRectPro::HAlign_Right:
				temp.Width(Width());
				temp.X(parentAbsRect.X() + parentAbsRect.Width() - Width());
				break;
			case XResource::XRectPro::HAlign_Stretch:
				temp.Width(parentAbsRect.Width() - X() - mX2);
				temp.X(parentAbsRect.X() + X());
				break;
			default:
				break;
			}
			switch (mVAlign)
			{
			case XResource::XRectPro::VAlign_Top:
				temp.Height(Height());
				temp.Y(parentAbsRect.Y() + Y());
				break;
			case XResource::XRectPro::VAlign_Center:
			{
				temp.Height(Height());
				double center = parentAbsRect.Y() + parentAbsRect.Height() / 2.0;
				center += (Y() - mY2);
				temp.Y(center - Height() / 2.0);
				break;
			}
			case XResource::XRectPro::VAlign_Bottom:
				temp.Height(Height());
				temp.Y(parentAbsRect.Y() + parentAbsRect.Height() - Height());
				break;
			case XResource::XRectPro::VAlign_Stretch:
				temp.Height(parentAbsRect.Height() - Y() - mY2);
				temp.Y(parentAbsRect.Y() + Y());
				break;
			default:
				break;
			}
			return temp;
		}
	private:
		double mX2 = 0.0, mY2 = 0.0;
		HAlignType mHAlign = HAlign_Left;
		VAlignType mVAlign = VAlign_Top;
	};
}