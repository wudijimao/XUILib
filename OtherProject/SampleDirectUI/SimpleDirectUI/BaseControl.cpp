#include"stdafx.h"
#include"BaseControl.h"

using namespace XControls;
using namespace XResource;

BaseControl::BaseControl(const std::wstring& id)
: mNeedReDraw(false),
mIsLButtonDown(false),
mDragWindowEnable(false),
mIsVisable(true)
{
	mBgkColor = Gdiplus::Color(0, 255, 255, 255);
	mId = id;
}
BaseControl::~BaseControl()
{
	//释放所有子控件，子控件智能指针无人持有 也会自动释放
	mChildrens.clear();
}
bool BaseControl::NeedReDraw()
{
	return mNeedReDraw;
}
void BaseControl::Draw(IXRender &g)
{
	mNeedReDraw = false;
	//先画自己
	if (!mIsVisable)
	{
		return;
	}
	if (mBgkImg.ImgPath() != L"")
	{
		g.DrawImage(mBgkImg, mAbsRect);
	}
	else if (mBgkColor.GetAlpha() != 0)
	{
		g.DrawBackGround(mBgkColor, mAbsRect);
	}
	//顺序画自己的子控件
	for (auto child : mRealChildrens)
	{
		child->Draw(g);
	}
}
const MouseStatusStruct& BaseControl::MouseStatus()
{
	return mBelongWnd->MouseStauts();
}
void BaseControl::KeyBoardEvent(UINT key, WPARAM wParam, LPARAM lParam)
{
	KeyDown(*this, wParam, lParam);
	for (auto child : mRealChildrens)
	{
		child->KeyBoardEvent(key, wParam, lParam);
	}
}
void BaseControl::MouseEvent(UINT key, int x, int y)
{
	x = x - mRect.IntX();
	y = y - mRect.IntY();
	mClickedX = x;//临时  应该定义一套完整的事件响应、记录机制
	mClickedY = y;

	//TODO::以后支持不规则区域点击测试（比如PNG透明区域不可点击）
	//鼠标已经在控件范围内按下了，或者鼠标在控件范围内
	if ((x > 0 && x< mRect.IntWidth() && y >0 && y < mRect.IntHeight()) || mIsLButtonDown)
	{
		switch (key)
		{
		case WM_LBUTTONDOWN:
			mIsLButtonDown = true;
			if (MouseDown)
			{
				MouseDown(*this);
			}
			if (!mHasFocus)
			{
				mHasFocus = true;
				if (GotFocus)
				{
					GotFocus(*this);
				}
			}
			if (mDragWindowEnable)
			{
				LPARAM lparm = (x << 16) && (y);
				mBelongWnd->Drag(lparm);
			}
			break;
		case WM_LBUTTONUP:
			if (mIsLButtonDown)
			{
				mIsLButtonDown = false;
				if (MouseClick)
				{
					MouseClick(*this);
				}
			}
			if (MouseUp)
			{
				MouseUp(*this);
			}
			break;
		case WM_MOUSEMOVE:
			if (!mIsMouseEntered)
			{
				mIsMouseEntered = true;
				if (MouseEnter) {
					MouseEnter(*this);
				}
			}
			if (MouseMove)
			{
				MouseMove(*this);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				//int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
				//MouseWheel(*this);
				break;
			}
		}
	}
	else
	{
		if (mIsMouseEntered)
		{
			mIsMouseEntered = false;
			//专为鼠标离开通知一次子控件
			for (auto child : mRealChildrens)
			{
				child->MouseEvent(key, x, y);
			}
			if (MouseLeave)
			{
				MouseLeave(*this);
			}
		}
		if (key == WM_LBUTTONDOWN && mHasFocus)
		{
			mHasFocus = false;
			if (LostFocus)
			{
				LostFocus(*this);
			}
		}
	}
	for (auto child : mRealChildrens)
	{
		child->MouseEvent(key, x, y);
	}
}


XResource::XRect& BaseControl::AbsRect()
{
	return mAbsRect;
}

const XResource::XRectPro& XControls::BaseControl::Rect() const
{
	return mRect;
}

XResource::XRectPro& BaseControl::Rect()
{
	return mRect;
}
void BaseControl::Rect(double x, double y, double width, double height)
{
	Rect(XResource::XRect(x, y, width, height));
}
void BaseControl::Rect(const XResource::XRect& rect)
{
	XResource::XRect temp = rect;
	//宽高必须大于等于0可以理解，  X，Y位置不能小于零的设定意思是不能超过父控件的范围，若超过范围则点击测试会覆盖不到超出父控件的部分
	if (rect.X() < 0 || rect.Y() < 0 || rect.Width() < 0 || rect.Height() < 0)
	{
		return;
	}
	if (PosChange &&( temp.X() != mRect.X() || temp.Y() != mRect.Y()))
	{
		PosChange(*this, temp);
	}
	if (SizeChange && (temp.Width() != mRect.Width() || temp.Height() != mRect.Height()))
	{
		SizeChange(*this, temp);
	}
	mRect = temp;
	RefreshAbsRect();
	SetReDrawFlag();
}

void XControls::BaseControl::Rect(const XResource::XRectPro& rect)
{
	mRect = rect;
	RefreshAbsRect();
	SetReDrawFlag();
}


void XControls::BaseControl::RefreshAbsRect()
{
	if (mId == L"middle")
	{
		int a = 0;
	}
	if (mParent != NULL)
	{
		AbsRect(mRect.MakeAbsRect(mParent->AbsRect()));
	}
	else
	{
		AbsRect(mRect);
	}
	for (auto child : mRealChildrens)
	{
		child->RefreshAbsRect();
	}
}
