#pragma once
#include "XRect.hpp"
#include <string>
#include <Windows.h>
#include <gdiplus.h>
namespace XResource
{
	enum class StretchEnum
	{
		None,		  //不拉伸（居中）
		Fill,         //拉伸填充
		Uniform,      //适应（有空白）
		UniformToFill //等比例缩放，填充
	};
	class XImage
	{
	public:
		XImage() :mNeedLoadImg(false){}
		XImage(const std::wstring &path) :mImgPath(path), mNeedLoadImg(true){}
		bool operator = (const std::wstring& imgUrl)
		{
			return ImgPath(imgUrl);
		}
		Gdiplus::Image* Image()
		{
			return mImg;
		}
		Gdiplus::CachedBitmap* CacheImage(Gdiplus::Graphics &g)
		{
			if (mNeedLoadDrawImg || mNeedLoadImg)
			{
				Gdiplus::Bitmap bitmap(mRect.IntWidth(), mRect.IntHeight());
				Gdiplus::Graphics *imgG = Gdiplus::Graphics::FromImage(&bitmap);
				imgG->DrawImage(mImg, 0, 0, mRect.IntWidth(), mRect.IntHeight());
				mCacheImg = new Gdiplus::CachedBitmap(&bitmap, &g);
				mNeedLoadImg = false;
				mNeedLoadDrawImg = false;
			}
			return mCacheImg;
		}
		///根据拉伸设置计算所需要绘制的区域，项目中所有rect都是相对于窗体的绝对区域
		XRect GetDrawRect(const XRect& rect)
		{
			if (mRect != rect)
			{
				mRect = rect;
				mNeedLoadDrawImg = true;
			}
			if (mNeedLoadImg)
			{
				if (mImg)
				{
					delete mImg;
				}
				mImg = new Gdiplus::Image(mImgPath.c_str());
			}
			return mRect;
		}
		///为空就是没有图片
		std::wstring ImgPath() const
		{
			return mImgPath;
		}
		bool ImgPath(const std::wstring &path)
		{
			if (mImgPath != path)
			{
				mImgPath = path;
				mNeedLoadImg = true;
				return true;
			}
			return false;
		}
		StretchEnum Stretch()
		{
			return mStretch;
		}
		void Stretch(StretchEnum stretch)
		{
			if (mStretch != stretch)
			{
				mStretch = stretch;
				mNeedLoadDrawImg = true;
			}
		}
	private:
		XRect mRect;
		bool mNeedLoadImg;
		bool mNeedLoadDrawImg = false;
		Gdiplus::CachedBitmap *mCacheImg;
		Gdiplus::Image *mImg = NULL;
		std::wstring mImgPath;
		StretchEnum mStretch;
	};
}