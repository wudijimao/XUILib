#pragma once
#include "IXCanvas.h"
#include <windows.h>
#include <gdiplus.h>

class GDICanvas : public IXCanvas
{
public:
	virtual RenderType GetType()
	{
		return RenderType::GDIPlus;
	}
	Gdiplus::Graphics *mGraphics;
};
