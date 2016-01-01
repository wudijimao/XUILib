#include "IRTClass.h"

class BaseRTClass :public IRTClass
{
public:
	virtual ~BaseRTClass()
	{
		OnDestory(this);
	}
	BaseRTClass(){}
	BaseRTClass(const std::string& name);
	virtual std::string GetObjName();
	virtual void SetObjName(const std::string& name);
private:
	std::string mObjName;
};