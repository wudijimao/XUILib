#pragma once
#include "../core/stdafx.hpp"
#include <cmath>

namespace XResource
{
    extern double gHighResolutionPixelScale;
    
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
		int mWidth = 0, mHeight = 0;
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
		int mX = 0, mY = 0;
	};
    
    class SIMPLEDIRECTUI_API XDisplaySize
    {
    public:
        bool operator == (const XDisplaySize &size) const {
            return !(*this != size);
        }
        bool operator != (const XDisplaySize &size) const {
            return (mWidth != size.mWidth || mHeight != size.mHeight);
        }
        bool operator < (const XDisplaySize &size) const {
            return (mWidth < size.mWidth && mHeight < size.mHeight);
        }
        bool operator <= (const XDisplaySize &size) const {
            return (mWidth <= size.mWidth && mHeight <= size.mHeight);
        }
        bool operator > (const XDisplaySize &size) const {
            return (mWidth > size.mWidth && mHeight > size.mHeight);
        }
        bool operator >= (const XDisplaySize &size) const {
            return (mWidth >= size.mWidth && mHeight >= size.mHeight);
        }
        XDisplaySize(){}
        XDisplaySize(double width, double height) {
            mWidth = width;
            mHeight = height;
        }
        void increaceWidth(double width) {
            mWidth += width;
        }
        void increaceHeight(double height) {
            mHeight += height;
        }
        void Width(double width) {
            mWidth = width;
        }
        void Height(double height) {
            mHeight = height;
        }
        int IntWidth() const {
            return (int)mWidth;
        }
        int IntHeight() const {
            return (int)mHeight;
        }
        double Width() const {
            return mWidth;
        }
        double Height() const {
            return mHeight;
        }
    private:
        double mWidth = 0.0, mHeight = 0.0;
    };
    
    
    
    class SIMPLEDIRECTUI_API XDisplayPoint
    {
    public:
        double quickDistance(const XDisplayPoint &point) const {
            return std::abs(std::pow(mX - point.mX, 2) + std::pow(mY - point.mY, 2));
        }
        double distance(const XDisplayPoint &point) const{
            return std::sqrt(quickDistance(point));
        }
        
        bool operator == (const XDisplayPoint &point) const {
            return !(*this != point);
        }
        bool operator != (const XDisplayPoint &point) const {
            return (mX != point.mX || mY != point.mY);
        }
        bool operator < (const XDisplayPoint &point) const {
            return (mX < point.mX && mY < point.mY);
        }
        bool operator <= (const XDisplayPoint &point) const {
            return (mX <= point.mX && mY <= point.mY);
        }
        bool operator > (const XDisplayPoint &point) const {
            return (mX > point.mX && mY > point.mY);
        }
        bool operator >= (const XDisplayPoint &point) const {
            return (mX >= point.mX && mY >= point.mY);
        }
        XDisplayPoint() {}
        XDisplayPoint(double x, double y) {
            mX = x;
            mY = y;
        }
        inline void moveX(double x) {
            mX += x;
        }
        inline void moveY(double y) {
            mY += y;
        }
        void X(double x) {
            mX = x;
        }
        void Y(double y) {
            mY = y;
        }
        const int intX() const {
            return (int)mX;
        }
        const int intY() const {
            return (int)mY;
        }
        const double X() const {
            return mX;
        }
        const double Y() const {
            return mY;
        }
    private:
        double mX = 0.0, mY = 0.0;
    };

	class SIMPLEDIRECTUI_API XRect
	{
	public:
        XRect &moveX(double x) {
            mPoint.moveX(x);
            return *this;
        }
        XRect &moveY(double y) {
            mPoint.moveY(y);
            return *this;
        }
        XRect &increaceWidth(double width) {
            mSize.increaceWidth(width);
            return *this;
        }
        XRect &increaceHeight(double height) {
            mSize.increaceHeight(height);
            return *this;
        }

        
        bool isPointIn(const XDisplayPoint &point) const {
            if (point >= mPoint
                && point.X() <= (X() + Width())
                && point.Y() <= (Y() + Height())) {
                return true;
            }
            return false;
        }
		XRect();
		XRect(double x, double y, double width, double height);
        XRect(const XDisplaySize&size);
		~XRect();
		bool operator == (const XRect &rect)
		{
			return !(*this != rect);
		}
		bool operator != (const XRect &rect)
		{
			return mPoint != mPoint || mSize != rect.mSize;
		}
		//以父窗口绝对位置为基数计算当前相对位置的绝对位置
		virtual XRect MakeAbsRect(const XRect& parentAbsRect) const;
        
        inline double rX() const {
            return mPoint.X() + mSize.Width();
        }
        inline double bY() const {
            return mPoint.Y() + mSize.Height();
        }

		inline void X(double x) {
			mPoint.X(x);
		}
		inline double X() const {
			return mPoint.X();
		}
		inline int IntX() const {
			return mPoint.intX();
		}
		inline void Y(double y) {
			mPoint.Y(y);
		}
		inline double Y() const {
			return mPoint.Y();
		}
		inline int IntY() const {
			return mPoint.intY();
		}
		inline void Width(double width) {
            mSize.Width(width);
		}
		inline double Width() const {
			return mSize.Width();
		}
		inline int IntWidth() const {
            return mSize.IntWidth();
		}
		inline void Height(double height) {
            mSize.Height(height);
		}
		inline double Height() const
		{
			return mSize.Height();
		}
		inline int IntHeight() const
		{
			return mSize.IntHeight();
		}
        const XDisplaySize& size() const {
            return mSize;
        }
        const XDisplayPoint& point() const {
            return mPoint;
        }
        inline void setSize(const XDisplaySize &size) {
            mSize = size;
        }
        inline void setPoint(const XDisplayPoint &point) {
            mPoint = point;
        }
	protected:
        XDisplaySize mSize;
        XDisplayPoint mPoint;
	};
    
    
    class SIMPLEDIRECTUI_API XEdge
    {
    public:
        XEdge() : mTop(0), mBottom(0), mLeft(0), mRight(0) {};
        XEdge(double left, double top, double right, double bottom) : mTop(top), mBottom(bottom), mLeft(left), mRight(right) {}
        void top(double top) {
            mTop = top;
        }
        void bottom(double bottom) {
            mBottom = bottom;
        }
        void left(double left) {
            mLeft = left;
        }
        void right(double right) {
            mRight = right;
        }
        
        void increaceTop(double top) {
            mTop += top;
        }
        void increaceBottom(double bottom) {
            mBottom += bottom;
        }
        void increaceLeft(double left) {
            mLeft += left;
        }
        void increaceRight(double right) {
            mRight += right;
        }
        
        double top() {
            return mTop;
        }
        double bottom() {
            return mBottom;
        }
        double left() {
            return mLeft;
        }
        double right() {
            return mRight;
        }
    private:
        double mTop;
        double mBottom;
        double mLeft;
        double mRight;
    };
}



