#pragma once
#include "IXRender.h"

class XRenderFactory {
public:
	static IXCanvas* getRender(HWND hwnd);
};