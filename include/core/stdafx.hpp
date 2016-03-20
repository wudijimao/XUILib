#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

#include "DllExport.hpp"

#ifdef TARGET_IPHONE_SIMULATOR
#ifndef TARGET_OS_IPHONE
#define TARGET_OS_IPHONE
#endif
#endif

#ifdef _WINDOWS
#ifndef TARGET_OS_MSWINDOWS
#define TARGET_OS_MSWINDOWS
#endif
#endif // WIN32



#ifdef TARGET_OS_IPHONE
#endif

#ifdef TARGET_OS_MAC
#endif

#ifdef TARGET_OS_MSWINDOWS
// Windows Header Files:
#include <windows.h>
#endif


