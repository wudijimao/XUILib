#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <windowsx.h>
#define GLEW_STATIC
#include "include\GL\glew.h"
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <iostream>
#include <fstream>

//#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

#include <assert.h>
#include <tchar.h>

#ifdef  assert
#define verify(expr) if(!expr) assert(0)
#else verify(expr) expr
#endif

const TCHAR szAppName[] = _T("TransparentGL");
const TCHAR wcWndName[] = _T("WS_EX_LAYERED OpenGL");

HDC hDC;
HGLRC m_hrc;
int w(240);
int h(240);;


BOOL initSC()
{
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 0);

	return 0;
}

void resizeSC(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

GLint _program;

BOOL renderSC()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	


	//glPushMatrix();

	//glColor3f(0, 1, 1);
	//glBegin(GL_TRIANGLES);                              // Drawing Using Triangles
	//glColor4f(1.0f, 0.0f, 0.0f, 0.5f);                      // Set The Color To Red
	//glVertex3f(0.0f, 1.0f, 0.0f);                  // Top
	//glColor3f(0.0f, 1.0f, 0.0f);                      // Set The Color To Green
	//glVertex3f(-1.0f, -1.0f, 0.0f);                  // Bottom Left
	//glColor3f(0.0f, 0.0f, 1.0f);                      // Set The Color To Blue
	//glVertex3f(1.0f, -1.0f, 0.0f);                  // Bottom Right
	//glEnd();


	/*glPopMatrix();
	glFlush();*/
	GLuint _vectexArrayObject;
	GLuint bufObjects[3];
	GLfloat _square[12];
	_square[0] = 0;
	_square[1] = 0;
	_square[2] = 0.5;

	_square[6] = 100;
	_square[7] = 0;
	_square[8] = 0.5;

	_square[9] = 100;
	_square[10] = 100;
	_square[11] = 0.5;

	_square[3] = 0;
	_square[4] = 100;
	_square[5] = 0.5;

	

	glGenBuffers(3, bufObjects);
	glGenVertexArrays(1, &_vectexArrayObject);
	glBindVertexArray(_vectexArrayObject);

	GLuint in = glGetAttribLocation(_program, "inPos");
	GLuint inC = glGetAttribLocation(_program, "inColor");

	GLfloat _color[16];
	GLfloat glColor[4] = { 255 / 255.0f, 1 / 255.0f, 1 / 255.0f, 255 / 255.0f };
	size_t size = sizeof(glColor);
	for (int i = 0; i < 4; ++i) {
		memcpy(&_color[i * 4], glColor, size);
	}

	glBindBuffer(GL_ARRAY_BUFFER, bufObjects[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_square), _square, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)in, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	/*
	glBindBuffer(GL_ARRAY_BUFFER, bufObjects[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_texturePos), _texturePos, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_TRUE, 0, nullptr);
*/

	glBindBuffer(GL_ARRAY_BUFFER, bufObjects[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_color), _color, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)inC, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	

	glEnableVertexAttribArray(in);
	glEnableVertexAttribArray(inC);
	//glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	glBindVertexArray(_vectexArrayObject);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glFlush();
	return 0;
}

bool checkShaderCompilState(GLuint shader) {
	GLint compileResult;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		//得到编译日志长度
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char *log = new char[logLen];
			GLsizei written;
			//得到日志信息并输出
			glGetShaderInfoLog(shader, logLen, &written, log);
			std::cout << log << std::endl;
			delete[] log;//释放空间
		}
		return false;
	}
	return true;
}
bool checkProgramCompilState(GLuint program) {
	GLint compileResult;
	glGetProgramiv(program, GL_LINK_STATUS, &compileResult);
	if (GL_FALSE == compileResult) {
		GLint logLen;
		//得到编译日志长度
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char *log = new char[logLen];
			GLsizei written;
			//得到日志信息并输出
			glGetProgramInfoLog(program, logLen, &written, log);
			std::cout << log << std::endl;
			delete[] log;//释放空间
		}
		return false;
	}
	return true;
}



bool init(const char *vertexShaderText, const char *fragmentShaderText) {
	if (vertexShaderText == nullptr || fragmentShaderText == nullptr) {
		return false;
	}
	// Create and compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderText, NULL);
	glCompileShader(vertexShader);
	if (!checkShaderCompilState(vertexShader))
	{
		return false;
	}

	// Create and compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderText, NULL);
	glCompileShader(fragmentShader);
	if (!checkShaderCompilState(fragmentShader))
	{
		return false;
	}

	// Create and link program
	_program = glCreateProgram();
	glAttachShader(_program, vertexShader);
	glAttachShader(_program, fragmentShader);
	glLinkProgram(_program);
	if (!checkProgramCompilState(_program))
	{
		return false;
	}
	// Use program
	glUseProgram(_program);
	return true;
}

void initShader() {
	std::ifstream in("VertexShader.vsh");
	std::istreambuf_iterator<char> beg(in), end;
	std::string vec(beg, end);

	std::ifstream in2("FragmentShader.fsh");
	std::istreambuf_iterator<char> beg2(in2), end2;
	std::string fragment(beg2, end2);
	init(vec.c_str(), fragment.c_str());
}



// DIB -> hDC
void draw(HDC pdcDest)
{
	//SwapBuffers(pdcDest);
}


BOOL CreateHGLRC()
{
	DWORD dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = dwFlags;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int PixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (PixelFormat == 0) {
		assert(0);
		return FALSE;
	}

	BOOL bResult = SetPixelFormat(hDC, PixelFormat, &pfd);
	if (bResult == FALSE) {
		assert(0);
		return FALSE;
	}

	m_hrc = wglCreateContext(hDC);
	if (!m_hrc) {
		assert(0);
		return FALSE;
	}

	wglMakeCurrent(hDC, m_hrc);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		return false;
	}

	return TRUE;
}

LRESULT CALLBACK WindowFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_ERASEBKGND:
		return 0;
		break;

	case WM_CREATE:
		break;

	case WM_DESTROY:
		if (m_hrc)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_hrc);
		}
		PostQuitMessage(0);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		renderSC(); // OpenGL -> DIB
		draw(hDC);  // DIB -> hDC
		EndPaint(hWnd, &ps);
		break;

	case WM_SIZE:
		w = LOWORD(lParam); h = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR str, int nWinMode)
{
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowFunc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hThisInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName = szAppName;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, _T("RegisterClassEx - failed"), _T("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	HWND hWnd = CreateWindowEx(WS_EX_LAYERED, szAppName, wcWndName,
		WS_VISIBLE | WS_POPUP, 200, 150, w, h,
		NULL, NULL, hThisInst, NULL);
	if (!hWnd) {
		MessageBox(NULL, _T("CreateWindowEx - failed"), _T("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	hDC = ::GetDC(hWnd);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_hrc);

	CreateHGLRC();
	verify(wglMakeCurrent(hDC, m_hrc));

	initSC();
	resizeSC(w, h);
	initShader();
	renderSC();

	verify(SetLayeredWindowAttributes(hWnd, 0x0, 0, LWA_COLORKEY));

	MSG msg;
	while (1)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else return 0;
		}
	}

	return (FALSE);
}
