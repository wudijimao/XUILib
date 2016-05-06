#pragma once
#include "stdafx.hpp"
#include "../res/XRect.hpp"

enum class RenderType
{
	GDIPlus,
	OPENGL
};

namespace XDUILib {
    class GLRenderData;
}


class SIMPLEDIRECTUI_API IXCanvas
{
public:
    static IXCanvas *gCurrentCanvas;
    virtual ~IXCanvas() {
        popCurrent();
    }
	virtual RenderType GetType() = 0;
    virtual void setSize(const XResource::XSize &size, double highResolutionPixelScale) = 0;
	virtual XResource::XSize GetSize() = 0;
    virtual void clear() = 0;
    virtual bool Present() = 0;
    virtual void pushRenderData(XDUILib::GLRenderData **data, int size) = 0;
    void makeCurrent() {
        gCurrentCanvas = this;
    }
    void popCurrent() {
        if (gCurrentCanvas == this) {
            if(mCanvasStack.size() > 0) {
                gCurrentCanvas = mCanvasStack.back();
                mCanvasStack.erase(std::next(mCanvasStack.rbegin()).base());
            } else {
                gCurrentCanvas = nullptr;
            }
        } else {
            auto rIter = mCanvasStack.rbegin();
            auto rEnd = mCanvasStack.rend();
            while (rIter != rEnd) {
                if (*rIter == this) {
                    mCanvasStack.erase(std::next(rIter).base());
                }
                ++rIter;
            }
        }
    }
private:
    static std::vector<IXCanvas*> mCanvasStack;
};