// TestRTClass.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../XCppRTClass/IRTClass.h"
#include "../XCppRTClass/RTClassManager.h"
#include <iostream>


class TestClass : public IRTClass
{
public:
	void Do()
	{
		std::cout << "DOOOO!!!" << std::endl;
		int a;
		std::cin >> a;
	}
	virtual void SetObjName(const std::string& name)
	{
		mName = name;
	}
	virtual std::string GetObjName()
	{
		return mName;
	}
	virtual void Regist(RTClassHolder &holder)
	{
		holder.RegistFun(TestClass::Do);
	}

private:
	std::string mName;
};


int _tmain(int argc, _TCHAR* argv[])
{
	TestClass c;
	c.SetObjName("abc");
	RTClassManager manager;
	manager.Regist(c);
	RTArgments args;
	RTValue ret;
	manager.CallFun("abc", "Do", args, ret);
	return 0;
}

