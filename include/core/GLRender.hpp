#pragma once
#include "IXRender.hpp"
#include "GLCanvas.hpp"

class SIMPLEDIRECTUI_API GLRender : public IXRender
{
public:
    virtual void setTransform3D(const GLTransform3D &transform) override;
    const GLTransform3D& getTransFrom3D() const;
    virtual void setPosition(const XResource::XDisplayPoint &point) override;
    virtual void move(const XResource::XDisplayPoint &point) override;
	virtual void Submit() override; //submit paint data to canvas
    virtual void clear() override;
	virtual void DrawBackGround(const XResource::XColor &color, const XResource::XRect &xRect) override;
    virtual void DrawBackGround(const XResource::XColor &color, const std::shared_ptr<XResource::IXImage> &image, const XResource::XRect &xRect) override;
    virtual void DrawImage(const std::shared_ptr<XResource::IXImage> &image, const XResource::XRect &rect) override;
    virtual void DrawImage(const std::shared_ptr<XResource::XStretchableImage> &image, const XResource::XRect &rect) override;
//	virtual void DrawLine(int x1, int y1, int x2, int y2);
//	//需要绘制的矩形区域
//	virtual void DrawImage(XResource::XImage &img, const XResource::XRect &rect);
//	//文本相关要单独处理？？ 如何与普通图形分开 又不过多多谢代码（分开的理由是OpenGl应该也会使用其渲染文本， 或者使用其他文字渲染引擎，比如FreeType/DirectWrite）
//	virtual void DrawString(const std::wstring &text, const XResource::XRect &xRect);
//	virtual void DrawString(const std::wstring &text, const XResource::XRect &xRect, Gdiplus::StringAlignment align);
    virtual void DrawString(const XResource::XAttributedString &attrStr, const XResource::XRect &xRect) override;
//	virtual XResource::XRect MeasureString(const XResource::XText &text, const XResource::XRect &xRect);
//	virtual XResource::XRect MeasureString(const std::wstring &text);
    
    virtual void setClipsToBounds(bool clips) override;
    //setBounds
    virtual void setBounds(const XResource::XRect &xRect) override;
    virtual void setMask(const std::shared_ptr<XResource::IXImage> &image) override;
private:
    bool mIsClipsToBounds;
    XResource::XRect mClipsRect;
    std::shared_ptr<XResource::IXImage> mClipsImage;
    std::vector<XDUILib::GLRenderData*> mCachedRenderData;
    GLTransform3D mTransform3D;
};

