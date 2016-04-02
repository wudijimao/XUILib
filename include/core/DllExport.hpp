#pragma once

#ifdef TARGET_OS_MSWINDOWS
    #ifdef XDUILIB_EXPORTS
    #define SIMPLEDIRECTUI_API __declspec(dllexport)
		#ifdef _DEBUG
			#define SIMPLEDIRECTUI_API_DEBUG __declspec(dllexport)
		#endif
    #else
    #define SIMPLEDIRECTUI_API __declspec(dllimport)
    #endif
#endif

#ifndef SIMPLEDIRECTUI_API
    #define SIMPLEDIRECTUI_API
#endif
#ifndef SIMPLEDIRECTUI_API_DEBUG
	#define SIMPLEDIRECTUI_API_DEBUG
#endif