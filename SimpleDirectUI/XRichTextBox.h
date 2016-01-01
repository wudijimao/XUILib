#pragma once
#include"BaseControl.h"
#include"XText.h"
#include<vector>
namespace XControls
{
	class SIMPLEDIRECTUI_API XRichTextBox : public BaseControl
	{
	public:
		XRichTextBox(const std::wstring& id);
		~XRichTextBox();
		virtual void Draw(IXRender &g)
		{
			XResource::XRect curentRect(0, 0, 0, 0);//当前位置
			XResource::XRect temp = mRect;
			for (auto text : mTexts)
			{
				curentRect = g.MeasureString(text, mRect);
				temp.Y(temp.Y() + curentRect.Height() + 5);
			}
			temp.Height(temp.Y() - mRect.Y());
			temp.Y(mRect.Y());
			Rect(temp);


			BaseControl::Draw(g);
			//g.DrawString(mText, AbsRect()/*, Gdiplus::StringAlignment::StringAlignmentCenter*/);
			temp = mRect;
			for (auto text : mTexts)
			{
				curentRect = g.MeasureString(text, mRect);
				g.DrawString(text, temp);
				temp.Y(temp.Y() + curentRect.Height() + 5);
			}
		}
		void AppendText(XResource::XText& text)
		{
			mTexts.push_back(text);
		}
	private:
		//std::wstring mText{L"dfdfds的说法独守空房教师节的飞洒的发的\r\n房价高的加工费的既视感觉得烦飞得更快间谍飞哥剪短发了"};
		std::vector<XResource::XText> mTexts;
	};

	
}