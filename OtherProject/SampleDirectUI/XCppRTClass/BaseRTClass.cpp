#include "stdafx.h"
#include "BaseRTClass.h"

BaseRTClass::BaseRTClass(const std::string& name)
{
	SetObjName(name);
}

std::string BaseRTClass::GetObjName()
{
	return mObjName;
}

void BaseRTClass::SetObjName(const std::string& name)
{
	mObjName = name;
}

