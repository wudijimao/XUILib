//#define GLEW_STATIC
#include <GL\glew.h>
#include <GL\GL.h>
#include<Windows.h>

struct GLColor {
	char r = '\0';
	char g = '\0';
	char b = '\0';
	char a = '\0';
};

void main()
{
	glewInit();
	HDC hdc;
	hdc = ::GetDC(m_hWnd);
	::ReleaseDC(m_hWnd, hdc)//一定要释放句柄
	GLuint colorBuf;
	glGenRenderbuffers(1, &colorBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, colorBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_SRGB8_ALPHA8, 100, 100);
	GLuint frameBuf;
	glGenFramebuffers(1, &frameBuf);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuf);

	int ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (ret == GL_FRAMEBUFFER_COMPLETE)
	{
		GLColor piexls[100 * 100];
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glReadPixels(0, 0, 100, 100, GL_RGBA, GL_UNSIGNED_BYTE, piexls);
	}
}