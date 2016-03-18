#pragma once

#ifdef TARGET_OS_MSWINDOWS
    #ifdef SIMPLEDIRECTUI_EXPORTS
    #define SIMPLEDIRECTUI_API __declspec(dllexport)
    #else
    #define SIMPLEDIRECTUI_API __declspec(dllimport)
    #endif
#else
    #define SIMPLEDIRECTUI_API
#endif