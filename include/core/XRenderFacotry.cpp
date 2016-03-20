#include "XRenderFactory.h"
#include "GDIRender.h"
#include "GLRender.h"

IXRender* XRenderFactory::getRender(HWND hwnd) {
	return new GLRender(hwnd);
	//return new GDIRender(hwnd);
}
