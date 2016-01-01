#if !defined(VER_INFO_DECLSPEC_H)
#define VER_INFO_DECLSPEC_H


#if defined(VER_INFO_STATIC)
#define VER_INFO_DECLSPEC
#elif defined(VER_INFO_EXPORTS)
#define VER_INFO_DECLSPEC _declspec(dllexport)
#else
#define VER_INFO_DECLSPEC _declspec(dllimport)
#endif


#endif
