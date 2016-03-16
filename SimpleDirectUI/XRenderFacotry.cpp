#include "XRenderFactory.h"
#include "GDIRender.h"
#include "GLRender.h"

IXCanvas* XRenderFactory::getRender(HWND hwnd) {
	//return new GLRender(hwnd);
	GDICanvas *canvas = new GDICanvas();
	if (canvas->Init(hwnd)) {
		return canvas;
	}
	return nullptr;
}
