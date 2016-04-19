#pragma once
#include "XResource.h"
//struct MouseStatusStruct
//{
//	bool IsLButtonDown, IsRButtonDown, IsXButtonDown, IsX2ButtonDown, IsMButtonDown;
//	MouseStatusStruct() :IsLButtonDown(false), IsRButtonDown(false), IsXButtonDown(false), IsX2ButtonDown(false), IsMButtonDown(false){};
//};
class GlobalStatus
{
public:
	GlobalStatus();
	~GlobalStatus();
	static GlobalStatus& GetInstance()
	{
		static GlobalStatus instance;
		return instance;
	}
	std::function<void(bool)> IsCaretShowChanged;
	void IsCaretShow(bool show)
	{
		mIsCaretShow = show;
		IsCaretShowChanged(mIsCaretShow);
	}
	bool IsCaretShow()
	{
		return mIsCaretShow;
	}
	//MouseStatusStruct& MouseStatus()
	//{
	//	return mMouseStatus;
	//}
	//XResource::XPoint& MousePos()
	//{
	//	return mMouseAbsPos;
	//}
private:
	bool mIsCaretShow;
	//MouseStatusStruct mMouseStatus;
	//XResource::XPoint mMouseAbsPos;
};

