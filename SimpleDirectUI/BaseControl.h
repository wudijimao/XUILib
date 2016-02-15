#pragma once
#include"stdafx.h"
#include "XImage.h"
#include "XRectPro.h"
#include "IBaseControl.h"
#include "IBaseWindow.h"
#include "DllExport.h"
#include<boost\shared_ptr.hpp>
#include<vector>

namespace XControls
{
	//TODO:写个ControlInterFace接口
	class SIMPLEDIRECTUI_API BaseControl : public IBaseControl
	{
	public:
		BaseControl(const std::wstring& id);
		~BaseControl();
		virtual void Draw(IXRender &g);
		virtual bool NeedReDraw();
		Gdiplus::Rect GetRect();
	protected:
		XResource::XImage mBgkImg;
		Gdiplus::Color mBgkColor;
	protected:
		IBaseControl* mParent = {NULL};//仅仅保存快捷链接
		//其他地方不允许持有子控件的智能指针，否则会导致控件树释放不完全
		std::vector<BaseControlPtr> mChildrens;//销毁时先从孩子销毁
	protected:
		virtual void AbsRect(const XResource::XRect& rect)
		{
			mAbsRect = rect;
		}
		virtual void AddChildIntenal(IBaseControl* ctrl)
		{
			ctrl->BelongWnd(&this->BelongWnd());
			ctrl->Parent(this);
			mRealChildrens.push_back(ctrl);
		}
		void SetReDrawFlag()
		{
			mNeedReDraw = true;
			if (mBelongWnd)
			{
				//TODO:If not transparent, parent need't redraw
				//If  opengl, parent need't run draw to recode info
				mBelongWnd->NeedReDraw();
			}
		}
	public:
		virtual const MouseStatusStruct& MouseStatus();
		virtual void MouseEvent(UINT key, int x, int y);
		virtual void KeyBoardEvent(UINT key, WPARAM wParam, LPARAM lParam);
		void Parent(IBaseControl* parent)
		{
			mParent = parent;
			RefreshAbsRect();
		}
		IBaseControl& Parent()
		{
			return *mParent;
		}
		///指针传进来之后请不要释放，全部交由内部管理 !!必须是new出来的 不能是取地址的，以后考虑用工厂创建控件
		void AddChild(IBaseControl* ctrl)
		{
			ctrl->BelongWnd(&this->BelongWnd());
			ctrl->Parent(this);
			mRealChildrens.push_back(ctrl);
			mChildrens.push_back(BaseControlPtr(ctrl));
		}

		void BkgColor(byte a, byte r, byte g, byte b)
		{
			mBgkColor = Gdiplus::Color(a, r, g, b);
			SetReDrawFlag();
		}
		void BkgImg(const std::wstring& path)
		{
			if (mBgkImg.ImgPath() != path)
			{
				mBgkImg.ImgPath(path);
				SetReDrawFlag();
			}
		}
		const std::wstring BkgImg()
		{
			return mBgkImg.ImgPath();
		}
		std::wstring ID()
		{
			return mId;
		}
		XResource::XRect& AbsRect();
		const XResource::XRectPro& Rect() const;
		XResource::XRectPro& Rect();
		void Rect(double x, double y, double width, double height);
		void Rect(const XResource::XRect& rect);
		void Rect(const XResource::XRectPro& rect);

		bool Visable()
		{
			return mIsVisable;
		}
		void Visable(bool visable)
		{
			if (mIsVisable != visable)
			{
				mIsVisable = visable;
				SetReDrawFlag();
			}
		}
		void DragWindowEnable(bool enable)
		{
			mDragWindowEnable = enable;
		}
		///获取控件是否可拖动窗体
		bool DragWindowEnable()
		{
			return mDragWindowEnable;
		}

		IXWindow& BelongWnd()
		{
			return *mBelongWnd;
		}
		bool BelongWnd(IXWindow *wnd)
		{
			mBelongWnd = wnd;
			for (auto child : mRealChildrens)
			{
				child->BelongWnd(wnd);
			}
			return true;
		}
	protected:
		IXWindow *mBelongWnd = {NULL};
		bool mNeedReDraw;
		XResource::XRectPro mRect;
		std::wstring mId;
		bool mIsLButtonDown;
		bool mHasFocus = false;
		bool mIsVisable;
		bool mDragWindowEnable;
		bool mIsMouseEntered = false;
		MouseStatusStruct mMouseStauts;
		//对窗体的绝对位置
		XResource::XRect mAbsRect;
		void RefreshAbsRect();
		virtual XResource::XPoint GetMousePos()
		{
			auto temp = mBelongWnd->MouseStauts();
			XResource::XPoint point(temp.X - mAbsRect.IntX(), temp.Y - mAbsRect.IntY());
			return point;
		}


		int mClickedX = 0, mClickedY = 0;//临时
	private:
		std::vector<IBaseControl*> mRealChildrens;//真正的全部children，包括控件内部创建的，不负责销毁
	};
}