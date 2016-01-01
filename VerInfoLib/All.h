#if !defined(VERSION_INFO_LIB_ALL_H)
#define VERSION_INFO_LIB_ALL_H

#pragma once

#if defined(_UNICODE)

#if defined(_DEBUG)
	#pragma comment(lib, "../debug/VerInfoLib.lib")
#else
	#pragma comment(lib, "../release/VerInfoLib.lib")
#endif

#else

#if defined(_DEBUG)
	#pragma comment(lib, "../../../output/debug/VerInfoLib.lib")
#else
	#pragma comment(lib, "../../../output/release/VerInfoLib.lib")
#endif
#endif
#include "DeclSpec.h"

#include "VersionInfo.h"
#endif