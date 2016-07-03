#pragma once
#include "../core/stdafx.hpp"
#include <gdiplus.h>
#include "../core/IXCanvas.hpp"


class GDICanvas : public IXCanvas
{
public:
	virtual RenderType GetType()
	{
		return RenderType::GDIPlus;
	}
	Gdiplus::Graphics *mGraphics;
};
