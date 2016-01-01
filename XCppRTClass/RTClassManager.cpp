#include "stdafx.h"
#include "RTClassManager.h"


void RTClassManager::Regist( IRTClass& obj )
{
	std::string className = typeid(obj).name();//根据编译器不同可能不同
#ifdef _DEBUG
	if (className.substr(0, 6) != "class ")
	{
		assert(false);
	}
#endif
	className = className.substr(6);
	boost::shared_ptr<RTClassHolder> holder(new RTClassHolder(className, obj.GetObjName(), (void*)&obj));
	obj.Regist(*(holder.get()));
	obj.OnDestory.connect(boost::bind(&RTClassManager::OnObjDestory, this, _1));
	mObjMap[obj.GetObjName()] = holder;
}

bool RTClassManager::CallFun(const std::string&objName, const std::string&funName, const RTArgments &args, RTValue& oRet)
{
	ObjMap::iterator iter = mObjMap.find(objName);
	if (iter != mObjMap.end())
	{
		return iter->second->CallFun(funName, args, oRet);
	}
	return false;
}

void RTClassManager::OnObjDestory( IRTClass* obj )
{
	ObjMap::iterator iter = mObjMap.find(obj->GetObjName());
	if (iter != mObjMap.end())
	{
		mObjMap.erase(iter);
	}
#ifdef _DEBUG
	else
	{
		assert(false);
	}
#endif
}
