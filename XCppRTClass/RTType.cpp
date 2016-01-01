#include "stdafx.h"
#include "RTType.h"

using namespace RTType;

Manager& Manager::GetInstance()
{
	static Manager rtType;
	return rtType;
}

RTTypeEnum Manager::GetType( const std::string& strType )
{
	if (mS2EMap.find(strType) != mS2EMap.end())
	{
		return mS2EMap[strType];
	}
	else
	{
		assert(0 && "UnKnonType:" && strType.c_str());
		return RetType_UnKnow;
	}
}

std::string Manager::GetType( RTTypeEnum type )
{
	if (mE2SMap.find(type) != mE2SMap.end())
	{
		return mE2SMap[type];
	}
	else
	{
		assert(0 && "UnKnonType:" && type);
		return UnKnowStr;
	}
}

Manager::Manager()
{
	mE2SMap[RetType_Void] = typeid(void).name();
	mS2EMap[typeid(void).name()] = RetType_Void;

	mE2SMap[RetType_Int] = typeid(int).name();
	mS2EMap[typeid(int).name()] = RetType_Int;

	mE2SMap[RetType_Bool] = typeid(bool).name();
	mS2EMap[typeid(bool).name()] = RetType_Bool;

	mE2SMap[RetType_Char] = typeid(char).name();
	mS2EMap[typeid(char).name()] = RetType_Char;

	mE2SMap[RetType_WChar] = typeid(wchar_t).name();
	mS2EMap[typeid(wchar_t).name()] = RetType_Int;

	mE2SMap[RetType_String] = typeid(std::string).name();
	mS2EMap[typeid(std::string).name()] = RetType_String;

	mE2SMap[RetType_WString] = typeid(std::wstring).name();
	mS2EMap[typeid(std::wstring).name()] = RetType_WString;

	mE2SMap[RetType_String] = typeid(const std::string&).name();
	mS2EMap[typeid(const std::string&).name()] = RetType_String;

	mE2SMap[RetType_WString] = typeid(const std::wstring&).name();
	mS2EMap[typeid(const std::wstring&).name()] = RetType_WString;

	mE2SMap[RetType_Long] = typeid(long).name();
	mS2EMap[typeid(long).name()] = RetType_Long;

	mE2SMap[RetType_CharPtr_C] = typeid(const char *).name();
	mS2EMap[typeid(const char *).name()] = RetType_CharPtr_C;

	mE2SMap[RetType_WharPtr_C] = typeid(const wchar_t *).name();
	mS2EMap[typeid(const wchar_t *).name()] = RetType_WharPtr_C;
}