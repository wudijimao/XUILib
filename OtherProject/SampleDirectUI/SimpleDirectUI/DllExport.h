#pragma once
#ifdef SIMPLEDIRECTUI_EXPORTS
#define SIMPLEDIRECTUI_API __declspec(dllexport)
#else
#define SIMPLEDIRECTUI_API __declspec(dllimport)
#endif