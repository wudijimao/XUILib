#include "GLRender.hpp"
//#include "GLHelper.hpp"
#include <vector>
#include <time.h>
#include <iterator>

bool GLRender::Init(IXCanvas *canvas) {
	return false;
}

void GLRender::Submit() {

}
void GLRender::DrawBackGround(const XResource::XColor &color, const XResource::XRect &xRect) {
}
//void GLRender::DrawLine(int x1, int y1, int x2, int y2) {
//
//}
////需要绘制的矩形区域
//void GLRender::DrawImage(XResource::XImage &img, const XResource::XRect &rect) {
//
//}
////文本相关要单独处理？？ 如何与普通图形分开 又不过多多谢代码（分开的理由是OpenGl应该也会使用其渲染文本， 或者使用其他文字渲染引擎，比如FreeType/DirectWrite）
//void GLRender::DrawString(const std::wstring &text, const XResource::XRect &xRect) {
//
//}
//void GLRender::DrawString(const std::wstring &text, const XResource::XRect &xRect, Gdiplus::StringAlignment align) {
//
//}
//void GLRender::DrawString(const XResource::XText &text, const XResource::XRect &xRect) {
//
//}
//XResource::XRect GLRender::MeasureString(const XResource::XText &text, const XResource::XRect &xRect) {
//	return xRect;
//}
//XResource::XRect GLRender::MeasureString(const std::wstring &text) {
//	return XResource::XRect(0, 0, 0, 0);
//}