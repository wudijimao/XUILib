#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>
#include <stack>
#include <memory>
#include <functional>
#include <algorithm>

#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
#ifndef TARGET_OS_IPHONE
#define TARGET_OS_IPHONE
#endif
#endif

#ifdef _WINDOWS
#ifndef TARGET_OS_MSWINDOWS
#define TARGET_OS_MSWINDOWS
#endif
#endif // WIN32

#if defined(ANDROID) || defined(__ANDROID__)
#define TARGET_OS_ANDROID
#endif



#ifdef TARGET_OS_IPHONE
#endif

#ifdef TARGET_OS_MAC
#endif

#ifdef TARGET_OS_MSWINDOWS
// Windows Header Files:
#include <WinSock2.h>
#define _WINSOCKAPI_
//#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <gdiplus.h>
#endif

#ifdef TARGET_OS_ANDROID
#include <initializer_list>
#include <jni.h>
#include <errno.h>
#include <cassert>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#endif


#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//#define new ::new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif // DEBUG

#include "DllExport.hpp"