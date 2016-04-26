#pragma once
#include "../core/stdafx.hpp"

namespace XResource
{
	class SIMPLEDIRECTUI_API XSize
	{
	public:
		XSize(){}
		XSize(int width, int height)
		{
			mWidth = width;
			mHeight = height;
		}
		void Width(int width)
		{
			mWidth = width;
		}
		void Height(int height)
		{
			mHeight = height;
		}
		int Width() const
		{
			return mWidth;
		}
		int Height() const
		{
			return mHeight;
		}
	private:
		int mWidth, mHeight;
	};

	class SIMPLEDIRECTUI_API XPoint
	{
	public:
		XPoint()
		{

		}
		XPoint(int x, int y)
		{
			mX = x;
			mY = y;
		}
		void X(int x)
		{
			mX = x;
		}
		void Y(int y)
		{
			mY = y;
		}
		const int& X() const
		{
			return mX;
		}
		const int& Y() const
		{
			return mY;
		}
	private:
		int mX, mY;
	};

	class SIMPLEDIRECTUI_API XRect
	{
	public:
        XRect &moveX(double x) {
            mX += x;
            return *this;
        }
        XRect &moveY(double y) {
            mY += y;
            return *this;
        }
        XRect &increaceWidth(double width) {
            mWidth += width;
            return *this;
        }
        XRect &increaceHeight(double height) {
            mHeight += height;
            return *this;
        }

        
        bool isPointIn(const XPoint &point) const {
            if (point.X() >= mX && point.Y() >= mY
                && point.X() <= (mX + mWidth)
                && point.Y() <= (mY + mHeight)) {
                return true;
            }
            return false;
        }
		XRect();
		XRect(double x, double y, double width, double height);
		~XRect();
		bool operator == (const XRect &rect)
		{
			return !(*this != rect);
		}
		bool operator != (const XRect &rect)
		{
			return ((mX != rect.mX) || (mY != rect.mY) || (mWidth != rect.mWidth) || (mHeight != rect.mHeight));
		}
		//以父窗口绝对位置为基数计算当前相对位置的绝对位置
		virtual XRect MakeAbsRect(const XRect& parentAbsRect) const;

		void X(double x)
		{
			mX = x;
		}
		double X() const
		{
			return mX;
		}
		int IntX() const
		{
			return (int)mX;
		}
		void Y(double y)
		{
			mY = y;
		}
		double Y() const
		{
			return mY;
		}
		int IntY() const
		{
			return (int)mY;
		}
		void Width(double width)
		{
			if (width >= 0.0)
			{
				mWidth = width;
			}
			else
			{
				mWidth = 0.0;
			}
		}
		double Width() const
		{
			return mWidth;
		}
		int IntWidth() const
		{
			return (int)mWidth;
		}
		void Height(double height)
		{
			if (height >= 0)
			{
				mHeight = height;
			}
		}
		double Height() const
		{
			return mHeight;
		}
		int IntHeight() const
		{
			return (int)mHeight;
		}
	protected:
		double mX = 0.0, mY = 0.0, mWidth = 0.0, mHeight = 0.0;
	};
}