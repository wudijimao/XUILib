#include "GLRender.h"
#include "GLHelper.h"
#include <vector>
#include <time.h>
#include <iterator>

struct GLColor {
	BYTE r = '\0';
	BYTE g = '\0';
	BYTE b = '\0';
	BYTE a = '\0';
};

void creatFramBuf() {
	GLuint colorBuf;
	glGenRenderbuffers(1, &colorBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, colorBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_SRGB8_ALPHA8, 100, 100);
	GLuint frameBuf;
	glGenFramebuffers(1, &frameBuf);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuf);

	int ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	GLColor piexls[100 * 100];
	if (ret == GL_FRAMEBUFFER_COMPLETE)
	{
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glReadPixels(0, 0, 100, 100, GL_RGBA, GL_UNSIGNED_BYTE, piexls);
	}
}

GLRender::GLRender(HWND hwnd) {
	GLHelper::init();
	GLHelper::winInit(hwnd);
	std::vector<char> buf;
	std::vector<char> buf2;
	char ch[120] = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
	clock_t t1 = clock();
	for (int i = 0; i < 100000; i++)
	{
		buf.insert(buf.end(), ch, ch + 120);
	}
	clock_t t2 = clock();
	for (int i = 0; i < 100000; i++)
	{
		std::copy(ch, ch + 120, std::back_inserter(buf2));
	}
	clock_t t3 = clock();
	clock_t m1 = t2 - t1;
	clock_t m2 = t3 - t2;
}

bool GLRender::Init(IXCanvas *canvas) {
	return false;
}

void GLRender::Creat() {

}

void GLRender::ReSize(int width, int height) {

}
void GLRender::Paint() {
}
void GLRender::Destory() {

}
void GLRender::DrawBackGround(Gdiplus::Color &color, const XResource::XRect &xRect) {

}
void GLRender::DrawLine(int x1, int y1, int x2, int y2) {

}
//需要绘制的矩形区域
void GLRender::DrawImage(XResource::XImage &img, const XResource::XRect &rect) {

}
//文本相关要单独处理？？ 如何与普通图形分开 又不过多多谢代码（分开的理由是OpenGl应该也会使用其渲染文本， 或者使用其他文字渲染引擎，比如FreeType/DirectWrite）
void GLRender::DrawString(const std::wstring &text, const XResource::XRect &xRect) {

}
void GLRender::DrawString(const std::wstring &text, const XResource::XRect &xRect, Gdiplus::StringAlignment align) {

}
void GLRender::DrawString(const XResource::XText &text, const XResource::XRect &xRect) {

}
XResource::XRect GLRender::MeasureString(const XResource::XText &text, const XResource::XRect &xRect) {
	return xRect;
}
XResource::XRect GLRender::MeasureString(const std::wstring &text) {
	return XResource::XRect(0, 0, 0, 0);
}