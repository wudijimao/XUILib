#pragma once
#include "RTValue.h"
#include <vector>
class RTArgments
{
public:
	template<typename T>
	T Get(int index) const
	{
		return mArgs[index].Get<T>();
	}
	int GetType(int index) const;
	int Size() const;
	template<typename T>
	void Add(T t)
	{
		mArgs.push_back(t);
	}
	void Clear();
private:
	std::vector<RTValue> mArgs;
};