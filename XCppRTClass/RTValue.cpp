#include "stdafx.h"
#include "RTValue.h"


using namespace RTType;

RTTypeEnum RTValue::GetType() const
{
	return mType;
}

void * RTValue::Get() const
{
	return mValuePtr;
}

void RTValue::Clear()
{
	mType = RTType::RetType_UnKnow;
}
