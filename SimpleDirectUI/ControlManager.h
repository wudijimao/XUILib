#pragma once
#include"BaseControl.h"
#include"stdafx.h"
#include<map>

namespace XControls
{
	typedef std::map<std::wstring, BaseControlPtr> ControlMap;

	//用来管理控件树，提供添加删除检索功能而已，不做其他功能
	class SIMPLEDIRECTUI_API ControlManager
	{
	public:
		ControlManager();
		~ControlManager();

		BaseControlPtr GetControl(const std::wstring& id);
		BaseControlPtr GetRoot();
	private:
		//控件map
		ControlMap mControls;
		//控件树根
		BaseControlPtr mRoot;
	};
}