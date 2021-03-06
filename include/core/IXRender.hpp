#pragma once

#include "GL/GLTransform3D.hpp"
#include "IXCanvas.hpp"
#include "../res/XResource.hpp"


class SIMPLEDIRECTUI_API IXRenderDataPovider {
public:
    virtual const GLTransform3D& rd_Transform() const = 0;
    
    virtual bool rd_NeedClipsChildren() const = 0;
    virtual int rd_ClipsLayerIndex() const = 0;
    virtual int rd_BeClipsDrawLayerIndex(bool isClipsChildren) const = 0;
};

class SIMPLEDIRECTUI_API IXRender
{
public:
    virtual const IXRenderDataPovider& getRenderDataPovider() const = 0;
	//virtual bool Init(IXCanvas *canvas) = 0;  use CurentCanvas
    virtual void clear() = 0;
	virtual void Submit() = 0;
	virtual void DrawBackGround(const XResource::XColor &color, const XResource::XRect &xRect, bool isClipsChildren = false) = 0;
    virtual void DrawBackGround(const XResource::XColor &color, const std::shared_ptr<XResource::IXImage> &image, const XResource::XRect &xRect, bool isClipsChildren = false) = 0;
    virtual void DrawImage(const std::shared_ptr<XResource::IXImage> &image, const XResource::XRect &rect) = 0;
    virtual void DrawImage(const std::shared_ptr<XResource::XStretchableImage> &image, const XResource::XRect &rect) = 0;
//	virtual void DrawLine(int x1, int y1, int x2, int y2) = 0;
	//需要绘制的矩形区域
//	virtual void DrawImage(XResource::XImage &img, const XResource::XRect &rect) = 0;
//	//文本相关要单独处理？？ 如何与普通图形分开 又不过多多谢代码（分开的理由是OpenGl应该也会使用其渲染文本， 或者使用其他文字渲染引擎，比如FreeType/DirectWrite）
//	virtual void DrawString(const std::wstring &text, const XResource::XRect &xRect) = 0;
//	virtual void DrawString(const std::wstring &text, const XResource::XRect &xRect, Gdiplus::StringAlignment align) = 0;
//	virtual void DrawString(const XResource::XText &text, const XResource::XRect &xRect) = 0;
//	virtual XResource::XRect MeasureString(const XResource::XText &text, const XResource::XRect &xRect) = 0;
//	virtual XResource::XRect MeasureString(const std::wstring &text) = 0;
    virtual void DrawString(const XResource::XAttributedString &attrStr, const XResource::XRect &xRect) = 0;
    virtual void setMask(const std::shared_ptr<XResource::IXImage> &image) = 0;
};