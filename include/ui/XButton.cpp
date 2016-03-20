#include "stdafx.h"
#include "XButton.h"
using namespace XControls;
XButton::XButton(const std::wstring& id) : BaseControl(id)
{
	MouseDown.connect(std::bind(&XButton::OnMouseDown, this, std::placeholders::_1));
	MouseUp.connect(std::bind(&XButton::OnMouseUp, this, std::placeholders::_1));
	MouseEnter.connect(std::bind(&XButton::OnMouseEnter, this, std::placeholders::_1));
	MouseLeave.connect(std::bind(&XButton::OnMouseLeave, this, std::placeholders::_1));
}
XButton::XButton(const std::wstring& id,
	const std::wstring& normalImg,
	const std::wstring& hoverImg,
	const std::wstring& downImg,
	const std::wstring& forceImg,
	const std::wstring& disableImg)
	:BaseControl(id),
	mNormalImg(normalImg),
	mDownImg(downImg),
	mHoverImg(hoverImg),
	mForceImg(forceImg),
	mDisableImg(disableImg)
{
	MouseDown.connect(std::bind(&XButton::OnMouseDown, this, std::placeholders::_1));
	MouseUp.connect(std::bind(&XButton::OnMouseUp, this, std::placeholders::_1));
	MouseEnter.connect(std::bind(&XButton::OnMouseEnter, this, std::placeholders::_1));
	MouseLeave.connect(std::bind(&XButton::OnMouseLeave, this, std::placeholders::_1));
}

XButton::~XButton()
{
}
void XButton::Draw(IXRender &g)
{
	BaseControl::Draw(g);
	//TODO:判断两遍 效率不高，应该根据是否改变和是否可见在递归前判断是否需要重绘，另外增加是否可点击到的属性IsClickTestEnable。
	if (!mIsVisable)
	{
		return;
	}
	XResource::XImage *img;
	switch (mState)
	{
	case XControls::ButtonStates::NORMAL:
		img = &mNormalImg;
		break;
	case XControls::ButtonStates::DOWN:
	case XControls::ButtonStates::HOVERDOWN:
		img = &mDownImg;
		break;
	case XControls::ButtonStates::HOVER:
		img = &mHoverImg;
		break;
	case XControls::ButtonStates::FORCE:
		img = &mForceImg;
		break;
	case XControls::ButtonStates::DISABLE:
	default:
		img = &mDisableImg;
		break;
	}
	g.DrawImage(*img, mAbsRect);
}
void XButton::OnMouseDown(IBaseControl&)
{
	if (mState == ButtonStates::HOVER)
	{
		mState = ButtonStates::HOVERDOWN;
	}
	else
	{
		mState = ButtonStates::DOWN;
	}
	SetReDrawFlag();
}
void XButton::OnMouseUp(IBaseControl&)
{
	if (mState == ButtonStates::HOVER)
	{
		return;
	}
	if (mState == ButtonStates::HOVERDOWN)
	{
		mState = ButtonStates::HOVER;
	}
	else
	{
		mState = ButtonStates::NORMAL;
	}
	SetReDrawFlag();
}
void XButton::OnMouseEnter(IBaseControl&)
{
	mState = ButtonStates::HOVER;
	SetReDrawFlag();
}
void XButton::OnMouseLeave(IBaseControl&)
{
	if (mState == ButtonStates::HOVERDOWN)
	{
		mState = ButtonStates::DOWN;
	}
	else
	{
		mState = ButtonStates::NORMAL;
		SetReDrawFlag();
	}
}