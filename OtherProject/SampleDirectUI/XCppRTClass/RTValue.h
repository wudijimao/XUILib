#pragma once
#include "RTType.h"
#include <boost/any.hpp>
#include <string>

class RTValue
{
public:
	template<typename T>
	RTValue(T t)
	{
		Set(t);
	}
	RTValue():mType(RTType::RetType_UnKnow){};
	RTType::RTTypeEnum GetType() const;
	template<typename T>
	T Get() const
	{
		return boost::any_cast<T>(mVal);
	}
	template<>
	int Get<int>() const
	{
		return mIntVal;
	}
	void *Get() const;
	void Set()
	{
		mType = RTType::RetType_Void;
	}
	void Set(int t)
	{
		mIntVal = t;
		mType = RTType::RetType_Int;
	}
	template<typename T>
	void Set(T t)
	{
		mVal = t;
		//mValuePtr = (void*)(boost::any_cast<T>(mVal));
		mType = RTType::Manager::GetInstance().GetType(typeid(t).name());
	}
	void Clear();
private:
	int mIntVal;
	RTType::RTTypeEnum mType;
	boost::any mVal;
	void *mValuePtr;
};