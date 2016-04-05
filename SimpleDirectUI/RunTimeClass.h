#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <assert.h>


class Object
{
public:
	virtual ~Object(){};
	Object(){};
};


template<class T>
class FunctionInfo
{
public:
	typedef void (T::*Func)(const std::string& val);
	FunctionInfo()
	{
	}
	void CallFunc(Object *c, const std::string& val)
	{
		((dynamic_cast<T*>(c))->*mFunc)(val);
	}
	Func mFunc;
private:
};
class ClassInfoBase
{
public:
	~ClassInfoBase(){};
	virtual Object* CreatInstance() = 0;
	virtual void CallFunction(Object* obj, const std::string& funcName, const std::string& val) = 0;
private:
};

template<class T, const std::string &Name>
class ClassInfo : public ClassInfoBase
{
public:
	ClassInfo()
	{
	}
	virtual Object* CreatInstance()
	{
		return CreatInctanceFunc();
	}
	virtual void RegistFun(const std::string& funcName, void (T::*func)(const std::string& val))
	{
		FunctionInfo<T> fInfo;
		fInfo.mFunc = func;
		mFunctions.insert(std::make_pair(funcName, fInfo));
	}
	virtual void CallFunction(Object* obj, const std::string& funcName, const std::string& val)
	{
		auto iter = mFunctions.find(funcName);
		if (iter != mFunctions.end())
		{
			iter->second.CallFunc(obj, val);
		}
		else
		{
#ifdef DEBUG
			assert(false && (L"函数名错误，找不到指定的函数：" + funcName).c_str());
#endif // DEBUG
		}
	}
	Object* (*CreatInctanceFunc)();
private:
	//暂时放在public
	std::map<std::string, FunctionInfo<T>> mFunctions;
	//保存静态的CreateInctance
	
};

class RunTimeInfo
{
public:
	static RunTimeInfo& GetInctance()
	{
		return mInctance;
	}
	RunTimeInfo()
	{
	}
	~RunTimeInfo()
	{
	}
	void Regist(const std::string& name, std::shared_ptr<ClassInfoBase> info)
	{
		mClassInfos.insert(std::make_pair(name, info));
	}
	Object* CreateInctance(const std::string& name)
	{
		auto iter = mClassInfos.find(name);
		if (iter != mClassInfos.end())
		{
			return iter->second->CreatInstance();
		}
#ifdef DEBUG
		assert(false && (L"类名错误，找不到指定的类：" + name).c_str());
#endif // DEBUG
		return NULL;
	}
	void CallFunction(Object*obj, const std::string& className, const std::string& funcName, const std::string& val)
	{
		auto iter = mClassInfos.find(className);
		if (iter != mClassInfos.end())
		{
			iter->second->CallFunction(obj, funcName, val);
		}
		else
		{
#ifdef DEBUG
			assert(false && (L"类名错误，找不到指定的类：" + className).c_str());
#endif // DEBUG
		}
	}
private:
	static RunTimeInfo mInctance;
	std::map<std::string, std::shared_ptr<ClassInfoBase>> mClassInfos;
};

template<class T, const std::string &Name>
class ClassRegister : public Object
{
public:
	ClassRegister()
	{
		mClassInfo.reset(new ClassInfo<class T, const std::string &Name>());
		mClassInfo->CreatInctanceFunc = CreateInstance;
		RunTimeInfo::GetInctance().Regist(Name, mClassInfo);
	}
	static Object* CreateInstance()
	{
		return new T;
	}
protected:
	std::shared_ptr<ClassInfo<T, Name>> *mClassInfo;
};

template<class T, const std::string &Name>
class FuncRegister
{
public:
	FuncRegister(ClassInfo<T, Name> *cInfo, const std::string &funcName, void (T::*func)(const std::string& val))
	{
		cInfo->RegistFun(funcName, func);
	};
	~FuncRegister(){};

private:

};