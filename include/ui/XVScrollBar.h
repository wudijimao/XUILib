#pragma once
#include "BaseControl.h"
#include "XButton.h"

namespace XControls
{
	class SIMPLEDIRECTUI_API XVScrollBar : public BaseControl
	{
	public:
		XVScrollBar(const std::wstring& id);
		~XVScrollBar();
		void ContentHeight(double height)
		{
			if (height < mRect.Height())
			{
				height = mRect.Height();
			}
			mContentHeight = height;

			double dragBarHeight = ((double)mRect.Height() / (double)mContentHeight) * mMiddleHeight;
			XResource::XRect temp = mDragBar.Rect();
			temp.Height(dragBarHeight);
			mDragBar.Rect(temp);

			mChangableY = mMiddleHeight - dragBarHeight;
		}

		boost::signals2::signal<void(double)> ScrollPostionChange;
	private:
		double CurrentContentPosition()
		{
			return (mCurrentY / mChangableY) * (mContentHeight - mRect.Height());
		}
		void SetCurrentY(double y)
		{
			if (y > mChangableY + 10.0)
			{
				mCurrentY = mChangableY + 10.0;
			}
			else if (y < 10.0)
			{
				mCurrentY = 10.0;
			}
			else
			{
				mCurrentY = y;
			}

			XResource::XRect temp(mDragBar.Rect());
			temp.Y(mCurrentY);
			mDragBar.Rect(temp);
		}
		double mContentHeight = { 0.0 };//整体高度
		double mMiddleHeight = { 0.0 };//中间空白处高度
		double mChangableY = { 0.0 };//可滚动Y
		double mCurrentY = { 0.0 };//当前滚动高度
		void OnSizeChange(IBaseControl &contrl, XResource::XRect &rect);
		void OnDrag(IBaseControl &contrl);
		XButton mUpBar = { L"upBar", L"img\\vbar\\vbar_up_normal.png", L"img\\vbar\\vbar_up_down.png", L"img\\vbar\\vbar_up_hover.png" };
		XButton mDownBar = { L"upBar", L"img\\vbar\\vbar_down_normal.png", L"img\\vbar\\vbar_down_down.png", L"img\\vbar\\vbar_down_hover.png" };
		XButton mDragBar = { L"upBar", L"img\\vbar\\vbar_drag_normal.png", L"img\\vbar\\vbar_drag_down.png", L"img\\vbar\\vbar_drag_hover.png" };
	};


}