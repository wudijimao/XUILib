#include "stdafx.h"
#include "XVScrollBar.h"
using namespace XControls;

XVScrollBar::XVScrollBar(const std::wstring& id) : BaseControl(id)
{
	AddChildIntenal(&mUpBar);
	AddChildIntenal(&mDownBar);
	AddChildIntenal(&mDragBar);

	mBgkImg.ImgPath(L"img\\vbar\\vbar_bkg.png");
	mUpBar.Rect(0, 0, 10, 10);
	mDownBar.Rect(0, 20, 10, 10);
	mDragBar.Rect(1, 10, 8, 10);
	mRect.Width(10);
	mRect.Height(30);
	mUpBar.MouseDown.connect([this](IBaseControl&){
		SetCurrentY(mCurrentY - 10.0);
		ScrollPostionChange(CurrentContentPosition());
	});
	mDownBar.MouseDown.connect([this](IBaseControl&){
		SetCurrentY(mCurrentY + 10.0);
		ScrollPostionChange(CurrentContentPosition());
	});
	mDragBar.MouseMove.connect(std::bind(&XVScrollBar::OnDrag, this, std::placeholders::_1));

	SizeChange.connect(std::bind(&XVScrollBar::OnSizeChange, this, std::placeholders::_1, std::placeholders::_2));
}

XVScrollBar::~XVScrollBar()
{
}
void XVScrollBar::OnDrag(IBaseControl &contrl)
{
	const MouseStatusStruct& status = this->MouseStatus();
	if (status.IsLButtonDown())
	{
		int changedY = status.Y - status.LastY;
		SetCurrentY(mCurrentY + changedY);
		ScrollPostionChange(CurrentContentPosition());
	}
}
void XVScrollBar::OnSizeChange(IBaseControl &contrl, XResource::XRect &rect)
{
	rect.Width(10);

	//调整下箭头按钮的位置
	XResource::XRect temp = mDownBar.Rect();
	temp.Y(rect.Height() - temp.Height());
	mDownBar.Rect(temp);

	mMiddleHeight = rect.Height() - 20;

	double dragBarHeight = (mRect.Height() / mContentHeight) * mMiddleHeight;
	temp = mDragBar.Rect();
	temp.Height(dragBarHeight);
	mDragBar.Rect(temp);

	mChangableY = mMiddleHeight - dragBarHeight;
}