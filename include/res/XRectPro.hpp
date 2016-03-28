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
			mX = rect.X();
			mY = rect.Y();
			mWidth = rect.Width();
			mHeight = rect.Height();
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
				temp.Width(mWidth);
				temp.X(parentAbsRect.X() + mX);
				break;
			case XResource::XRectPro::HAlign_Center:
			{
				temp.Width(mWidth);
				//中心点
				double center = parentAbsRect.X() + parentAbsRect.Width() / 2.0;
				//中心点根据X X2 偏移
				center += (mX - mX2);
				temp.X(center - mWidth / 2.0);
				break;
			}	
			case XResource::XRectPro::HAlign_Right:
				temp.Width(mWidth);
				temp.X(parentAbsRect.X() + parentAbsRect.Width() - mWidth);
				break;
			case XResource::XRectPro::HAlign_Stretch:
				temp.Width(parentAbsRect.Width() - mX - mX2);
				temp.X(parentAbsRect.X() + mX);
				break;
			default:
				break;
			}
			switch (mVAlign)
			{
			case XResource::XRectPro::VAlign_Top:
				temp.Height(mHeight);
				temp.Y(parentAbsRect.Y() + mY);
				break;
			case XResource::XRectPro::VAlign_Center:
			{
				temp.Height(mHeight);
				double center = parentAbsRect.Y() + parentAbsRect.Height() / 2.0;
				center += (mY - mY2);
				temp.Y(center - mHeight / 2.0);
				break;
			}
			case XResource::XRectPro::VAlign_Bottom:
				temp.Height(mHeight);
				temp.Y(parentAbsRect.Y() + parentAbsRect.Height() - mHeight);
				break;
			case XResource::XRectPro::VAlign_Stretch:
				temp.Height(parentAbsRect.Height() - mY - mY2);
				temp.Y(parentAbsRect.Y() + mY);
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