#include "JsonReader.h"
#include <stdio.h>
#include <stdlib.h>
using namespace PlatformAPI;

long long StringAddFuns::ToInt64(std::string str)
{
	return _atoi64(str.c_str());
}
long long StringAddFuns::ToInt64(std::wstring str)
{
	return _wtoi64(str.c_str());
}