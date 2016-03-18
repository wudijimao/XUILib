#pragma once
#include "BaseControl.h"
#include "DllExport.h"
namespace XControls
{
	enum class ButtonStates
	{
		NORMAL = 0,
		DOWN,
		HOVER,
		HOVERDOWN,
		FORCE,
		DISABLE
	};
	class SIMPLEDIRECTUI_API XButton : public BaseControl
	{
	public:
		XButton(const std::wstring& id);
		XButton(const std::wstring& id,
			const std::wstring& normalImg,
			const std::wstring& downImg = L"",
			const std::wstring& hoverImg = L"",
			const std::wstring& forceImg = L"",
			const std::wstring& disableImg = L"");
		~XButton();
		virtual void Draw(IXRender &g);
		const XResource::XImage& NormalImg()
		{
			return mNormalImg;
		}
		void NormalImg(const std::wstring& img)
		{
			mNormalImg = img;
		}
		const XResource::XImage& DownImg()
		{
			return mDownImg;
		}
		void DownImg(const std::wstring& img)
		{
			mDownImg = img;
		}
		const XResource::XImage& HoverImg()
		{
			return mHoverImg;
		}
		void HoverImg(const std::wstring& img)
		{
			mHoverImg = img;
		}
		const XResource::XImage& ForceImg()
		{
			return mForceImg;
		}
		void ForceImg(const std::wstring& img)
		{
			mForceImg = img;
		}
		const XResource::XImage& DisableImg()
		{
			return mDisableImg;
		}
		void DisableImg(const std::wstring& img)
		{
			mDisableImg = img;
		}
	private:
		void OnMouseDown(IBaseControl&);
		void OnMouseUp(IBaseControl&);
		void OnMouseEnter(IBaseControl&);
		void OnMouseLeave(IBaseControl&);
	private:
		ButtonStates mState{ ButtonStates::NORMAL };
		XResource::XImage mNormalImg, mDownImg, mHoverImg, mForceImg, mDisableImg;
	};
}

