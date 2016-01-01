#pragma once
#include "XRect.h"

enum class RenderType
{
	GDIPlus,
	OPENGL
};

class IXCanvas
{
public:
	virtual RenderType GetType() = 0;
	virtual XResource::XSize GetSize() = 0;
private:

};