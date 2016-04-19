#pragma once
#include "IXCanvas.h"
#include "GDICanvas.h"

//Win

class GLSoftwareCanvas : public GDICanvas
{
public:
	virtual RenderType GetType()
	{
		return RenderType::GDIPlus;
	}
	Gdiplus::Graphics *mGraphics;
};
