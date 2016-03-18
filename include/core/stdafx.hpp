#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "DllExport.hpp"

#ifdef TARGET_IPHONE_SIMULATOR
#ifndef TARGET_OS_IPHONE
#define TARGET_OS_IPHONE
#endif
#endif


#ifdef TARGET_OS_IPHONE
#endif

#ifdef TARGET_OS_MAC
#endif

#ifdef TARGET_OS_MSWINDOWS
// Windows Header Files:
#include <windows.h>
#endif


