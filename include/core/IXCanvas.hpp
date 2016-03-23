#pragma once
#include "../res/XText.hpp"
#include "../res/XRect.hpp"

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
    virtual bool Present() = 0;
protected:

private:

};