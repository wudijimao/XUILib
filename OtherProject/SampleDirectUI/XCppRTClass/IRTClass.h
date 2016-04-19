#pragma once
#include "RTClassHolder.h"
#include <string>
#include <map>
#include <boost/signals2.hpp>

class IRTClass
{
public:
	virtual ~IRTClass(){}
	virtual std::string GetObjName() = 0;
	virtual void SetObjName(const std::string& name) = 0;
	virtual void Regist(RTClassHolder &holder) = 0;
	boost::signals2::signal<void(IRTClass*)> OnDestory;
};