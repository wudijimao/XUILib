#include "stdafx.h"
#include "RTArgments.h"

int RTArgments::GetType(int index) const
{
	return mArgs[index].GetType();
}

int RTArgments::Size() const
{
	return mArgs.size();
}

void RTArgments::Clear()
{
	mArgs.clear();
}
