#pragma once
#include "BaseControl.h"
#include "XText.h"
#include "GlobalStatus.h"
#include <Windows.h>
namespace XControls
{
	class SIMPLEDIRECTUI_API XTextBox : public BaseControl
	{
	public:
		XTextBox(const std::wstring& id) : BaseControl(id)
		{
			GlobalStatus::GetInstance().IsCaretShowChanged = std::bind(&XTextBox::IsCaretShowChanged, this, std::placeholders::_1);
			MouseDown=std::bind(&XTextBox::OnMouseDown, this, std::placeholders::_1);
			GotFocus=std::bind(&XTextBox::OnGotFocus, this, std::placeholders::_1);
			LostFocus=std::bind(&XTextBox::OnLostFocus, this, std::placeholders::_1);
			KeyDown=std::bind(&XTextBox::OnKeyDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		}
		XResource::XText& Text()
		{
			return mText;
		}
		void IsEditEnable(bool enable)
		{
			mIsEditEnable = enable;
		}
		bool IsEditEnable()
		{
			return mIsEditEnable;
		}
	private:
		void IsCaretShowChanged(bool)
		{
			SetReDrawFlag();
		}
		void Draw(IXRender &g)
		{
			BaseControl::Draw(g);
			g.DrawString(mText, mAbsRect);
			if (mHasCaret && GlobalStatus::GetInstance().IsCaretShow())
			DrawCaret(g);
		}
		void DrawCaret(IXRender &g)
		{
			{
				if (IsClicked)
				{
					IsClicked = false;
					auto temp = mText;
					XResource::XRect rect;
					for (int i = mText.mText.length(); i >= 0; --i)
					{
						temp.mText = mText.mText.substr(0, i);
						rect = g.MeasureString(temp, mAbsRect);
						if (mClickPoint.X() >= rect.Width())
						{
							break;
						}
					}
					mCaretX = rect.Width();
				}
				double x = mAbsRect.X() + mCaretX;
				double y1 = mAbsRect.Y();
				g.DrawLine((int)x, (int)y1, (int)x, (int)y1 + mRect.IntHeight());
			}
		}
		bool mIsEditEnable = true;
		double mCaretX;//相对X坐标
		double mCaretPos;//第几个文字，从0开始
		bool mHasCaret = false;
		void OnMouseDown(IBaseControl& contrl)
		{
			IsClicked = true;
			mClickPoint = GetMousePos();
		}
		void OnGotFocus(IBaseControl& contrl)
		{
			//获得焦点
			if (mIsEditEnable)
			{
				mHasCaret = true;
				SetReDrawFlag();
			}
		}
		void OnLostFocus(IBaseControl& contrl)
		{
			if (mHasCaret)
			{
				mHasCaret = false;
				SetReDrawFlag();
			}
		}
		void OnKeyDown(IBaseControl&, WPARAM wParam, LPARAM lParam)
		{
			if (mHasCaret)
			{
				if (wParam == VK_BACK)
				{
					mText.mText = mText.mText.substr(0, mText.mText.length() - 1);
				}
				if (wParam == VK_OEM_PERIOD || wParam == VK_DECIMAL)
				{
					mText.mText += '.';
				}
				//数字键0-9
				if (wParam >= '0' && wParam <= '9')
				{
					mText.mText += (char)wParam;
				}
				//小键盘0-9
				wParam -= 0x30;
				if (wParam >= '0' && wParam <= '9')
				{
					mText.mText += (char)wParam;
				}
				SetReDrawFlag();
			}
		}
		XResource::XText mText;
		//鼠标点击的位置
		bool IsClicked = false;
		XResource::XPoint mClickPoint;
	};
}