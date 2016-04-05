#include "stdafx.h"
#include "XCheckBox.h"


XControls::XCheckBox::XCheckBox(const std::wstring& id, bool isChecked /*= false*/) : BaseControl(id),
mIsChecked(isChecked),
mBtnChecked(id + L"_Checked", L"img\\checkbox\\check_normal.png", L"img\\checkbox\\check_hover.png", L"img\\checkbox\\check_down.png"),
mBtnUnChecked(id + L"_UnChecked", L"img\\checkbox\\uncheck_normal.png", L"img\\checkbox\\uncheck_hover.png", L"img\\checkbox\\uncheck_down.png"),
mText(id + L"_Text")
{
	mBtnChecked.Visable(mIsChecked);
	mBtnUnChecked.Visable(!mIsChecked);
	MouseUp = std::bind(&XCheckBox::OnMouseUp, this, std::placeholders::_1);
	AddChildIntenal(&mBtnChecked);
	AddChildIntenal(&mBtnUnChecked);
	AddChildIntenal(&mText);
	mBtnChecked.Rect(0, 0, 20, 20);
	mBtnUnChecked.Rect(0, 0, 20, 20);
	mText.Rect(25, 3, 200, 20);
}

void XControls::XCheckBox::OnMouseUp(IBaseControl&)
{
	mIsChecked = !mIsChecked;
	mBtnChecked.Visable(mIsChecked);
	mBtnUnChecked.Visable(!mIsChecked);
	SetReDrawFlag();
}

XResource::XText& XControls::XCheckBox::Text()
{
	return mText.Text();
}

bool XControls::XCheckBox::IsChecked()
{
	return mIsChecked;
}
