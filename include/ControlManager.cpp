#include "stdafx.h"
#include "ControlManager.h"

using namespace XControls;


ControlManager::ControlManager()
{
	mRoot.reset(new BaseControl(L"Root"));
}

ControlManager::~ControlManager()
{
	//必须保证先清除这个列表，控件树才会在树根销毁后自动全部销毁
	mControls.clear();
}


BaseControlPtr ControlManager::GetControl(const std::wstring& id)
{
	ControlMap::iterator iter = mControls.find(id);
	if (iter != mControls.end())
	{
		return iter->second;
	}
	return BaseControlPtr();
}
BaseControlPtr ControlManager::GetRoot()
{
	return mRoot;
}