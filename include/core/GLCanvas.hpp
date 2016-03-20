#pragma once
#include "IXCanvas.hpp"

class GDICanvas : public IXCanvas
{
public:
	virtual RenderType GetType()
	{
		return RenderType::GDIPlus;
	}
	Gdiplus::Graphics *mGraphics;
};
