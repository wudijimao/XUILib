#pragma once
#include "IXRender.h"

class XRenderFactory {
public:
	static IXRender* getRender(HWND hwnd);
};