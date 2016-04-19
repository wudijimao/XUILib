#pragma once
#include "BaseRTClassHolder.h"
#include "RTArgments.h"
#include <string>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

class Base
{
public:
	virtual void operator()(const RTArgments &args, RTValue &ret) = 0;
private:
	
};

template<typename R>
class BaseTemp0 : public Base
{
public:
	virtual void operator()(const RTArgments &args, RTValue &ret)
	{
		ret.Set(mFunc());
	}
	BaseTemp0(){}
	BaseTemp0(const boost::function<R(void)> &fun)
	{
		mFunc = fun;
	}
	boost::function<R(void)> mFunc;
};
template<>
class BaseTemp0<void> : public Base
{
public:
	virtual void operator()(const RTArgments &args, RTValue &ret)
	{
		mFunc();
		ret.Set();
	}
	BaseTemp0(){}
	BaseTemp0(boost::function<void(void)> &fun)
	{
		mFunc = fun;
	}
	virtual void Clear(){}
	boost::function<void(void)> mFunc;
};


template<typename R, typename T1>
class BaseTemp1 : public Base
{
public:
	BaseTemp1(boost::function<R(T1)> &fun)
	{
		mFunc = fun;
	}
	boost::function<R(T1)> mFunc;
	virtual void operator()(const RTArgments &args, RTValue &ret)
	{
		ret.Set(mFunc(args.Get<T1>(0)));
	}
};

template<typename R, typename T1, typename T2>
class BaseTemp2 : public Base
{
public:
	BaseTemp2(boost::function<R(T1, T2)> &fun)
	{
		mFunc = fun;
	}
	boost::function<R(T1, T2)> mFunc;
	virtual void operator()(const RTArgments &args, RTValue &ret)
	{
		ret.Set(mFunc(args.Get<T1>(0), args.Get<T2>(1)));
	}
};

template<typename R, typename T1, typename T2, typename T3>
class BaseTemp3 : public Base
{
public:
	BaseTemp3(boost::function<R(T1, T2, T3)> &fun)
	{
		mFunc = fun;
	}
	virtual void operator()(const RTArgments &args, RTValue &ret)
	{
		ret.Set(mFunc(args.Get<T1>(0), args.Get<T2>(1), args.Get<T3>(2)));
	}
	boost::function<R(T1, T2, T3)> mFunc;
};

typedef boost::shared_ptr<Base> BasePtr;
class RTClassManagerWithTemplate : public BaseRTClassHolder
{
public:
	RTClassManagerWithTemplate(const std::string& className, const std::string& objName, void* obj) : BaseRTClassHolder(className, objName, obj){}

	bool CallFun(const std::string funName, const RTArgments &args, RTValue &oRet)
	{
		int index = GetFunIndex(funName, args);
		if (index != -1)
		{
			BasePtr func = mFuns[index];
			(*func)(args, oRet);
			return true;
		}
		return false;
	}
	template <typename CppFunc>
	void RegistFunInternal(CppFunc add, const char*name)
	{	
		Reg(add);
		BaseRTClassHolder::RegistFunInternal(add, name);
	}
private:
	template<typename R, class T0>
	void Reg(R(T0::*fun)())
	{
		boost::function<R(void)> bindFun = boost::bind(fun, (T0*)mObj);
		BasePtr ptr(new BaseTemp0<R>(bindFun));
		mFuns.push_back(ptr);
	}
	template<typename R, class T0, typename T1>
	void Reg(R(T0::*fun)(T1))
	{
		boost::function<R(T1)> bindFun = boost::bind(fun, (T0*)mObj, _1);
		BasePtr ptr(new BaseTemp1<R, T1>(bindFun));
		mFuns.push_back(ptr);
	}
	template<typename R, class T0, typename T1, typename T2>
	void Reg(R(T0::*fun)(T1, T2))
	{
		boost::function<R(T1, T2)> bindFun = boost::bind(fun, (T0*)mObj, _1, _2);
		BasePtr ptr(new BaseTemp2<R, T1, T2>(bindFun));
		mFuns.push_back(ptr);
	}
	template<typename R, class T0, typename T1, typename T2, typename T3>
	void Reg(R(T0::*fun)(T1, T2, T3))
	{
		boost::function<R(T1, T2, T3)> bindFun = boost::bind(fun, (T0*)mObj, _1, _2, _3);
		BasePtr ptr(new BaseTemp3<R, T1, T2, T3>(bindFun));
		mFuns.push_back(ptr);
	}
private:
	
	std::vector<BasePtr>mFuns;
};