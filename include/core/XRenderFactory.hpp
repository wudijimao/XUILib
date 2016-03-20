#pragma once
#include "IXRender.hpp"

class XRenderFactory {
public:
	static IXRender* getRender(HWND hwnd);
};