#pragma once
//#define USE_ASM

#ifdef USE_ASM
	#include "RTClassHolderWithASM.h"
	typedef RTClassHolderWithASM RTClassHolder;
#else
	#include "RTClassManagerWithTemplate.h"
	typedef RTClassManagerWithTemplate RTClassHolder;
#endif
