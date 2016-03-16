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
	virtual void Update() = 0;//¸üÐÂÏÔÊ¾
	virtual void ReSize(int width, int height) = 0;
protected:

private:

};