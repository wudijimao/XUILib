#include "stdafx.h"
#include "BaseRTClassHolder.h"

BaseRTClassHolder::BaseRTClassHolder( const std::string& className, const std::string& objName, void* obj )
{
	mClassName = className;
	mObjName = objName;
	mObj = obj;
}
const std::string& BaseRTClassHolder::GetClassName()
{
	return mClassName;
}

const std::string& BaseRTClassHolder::GetObjName()
{
	return mObjName;
}

void* BaseRTClassHolder::GetObj()
{
	return mObj;
}

std::string BaseRTClassHolder::GetFunName( const std::string &funcFullName )
{
	std::string name = "";
	int index = funcFullName.find_last_of(':');
	if (index != -1)
	{
		++index;
		//如果是子类父类 会导致类名不同，暂时不判断是否是一个类的函数
		//std::string className = funcFullName.substr(0, index - 2);
		//if (className == mClassName)
		//{
		name = funcFullName.substr(index, funcFullName.length() - index);
		//}
	}
	return name;
}

void BaseRTClassHolder::SaveFunType( std::string &typeStr, FuncStrSign& func )
{
	int index = typeStr.find_first_of('(');
	if (index>0)
	{
		std::string retStr = typeStr.substr(0, index - 1);
		func.mReturnType = RTType::Manager::GetInstance().GetType(retStr);
		index = typeStr.find_first_of(')');
		if (index > 0)
		{
			++index;
			typeStr = typeStr.substr(index);
			if (typeStr.c_str()[0] == '(' && typeStr.c_str()[typeStr.length() - 1] == ')')
			{
				typeStr = typeStr.substr(1, typeStr.length() - 2);
				std::string argName;
				do
				{
					index = typeStr.find_first_of(",");
					if (index != -1)
					{
						argName = typeStr.substr(0, index);
						typeStr = typeStr.substr(index + 1);
					}
					else
					{
						argName = typeStr;
					}
					if (argName != "void")
					{
						func.mArgTypes.push_back(RTType::Manager::GetInstance().GetType(argName));
					}
				} while (index != -1);
				return;
			}
		}
	}
	assert(false);
}


int BaseRTClassHolder::GetFunIndex( const std::string funName, const RTArgments &args )
{
	std::vector<FuncStrSign>::iterator iter = mFunSigns.begin();
	std::vector<FuncStrSign>::iterator end = mFunSigns.end();
	int i = 0;
	for (; iter != end; ++iter, ++i)
	{
		if (iter->mName == funName && args.Size() == iter->mArgTypes.size())
		{
			bool isSame = true;
			for (int j = args.Size() - 1; j >= 0; --j)
			{
				if (args.GetType(j) != iter->mArgTypes[j])
				{
					isSame = false;
					break;
				}
			}
			if (isSame)
			{
				return i;
			}
		}
	}
	return -1;
}
