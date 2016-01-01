#pragma once
#include "IXCanvas.h"
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
