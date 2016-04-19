#pragma once
#include <string>
#include <map>

namespace RTType
{
	enum RTTypeEnum
	{
		RetType_UnKnow = -1,
		RetType_Void = 0,
		RetType_Bool = 1,
		RetType_VoidPtr = 2,
		RetType_Int = 3,
		RetType_CharPtr_C = 4,
		RetType_AnyVector = 5,
		RetType_Long = 6,
		RetType_VoidPtr_C = 7,
		RetType_LUA_TTHREAD = 8,
		RetType_WharPtr_C,
		RetType_Char,
		RetType_WChar,
		RetType_String,
		RetType_WString,
		RetType_StringRef_C,
		RetType_WStringRef_C,
		RetType_LuaPtr
	};
	static const std::string UnKnowStr = "RetType_UnKnow";

	//提供类型标准，与typeid.name对应
	class Manager
	{
	public:
		static Manager& GetInstance();
		RTTypeEnum GetType(const std::string& strType);
		std::string GetType(RTTypeEnum type);
	private:
		Manager();
		std::map<RTTypeEnum, std::string>mE2SMap;
		std::map<std::string, RTTypeEnum>mS2EMap;
	};

}