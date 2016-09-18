#include "../../include/core/MutiThread/XRunLoop.hpp"
#include "../../include/core/XApp.hpp"

static const unsigned int WM_USER_RunLoopMsg = WM_USER + 1152;
static std::shared_ptr<IXRunLoop> gXMainRunloop = std::make_shared<XMainRunloop>();


std::shared_ptr<IXRunLoop> getMainRunLoop() {
	return gXMainRunloop;
}

void XMainRunloop::run() {
}
bool XMainRunloop::weakUp(XRunLoopSource *source) {
	IXRunLoop::weakUp(source);
	PostMessage(XDUILib::XApp::thisApp().mainWindow()->getHandler(), WM_USER_RunLoopMsg, 0, 0);
	return true;
}
void XMainRunloop::wait() {
}
bool XMainRunloop::waitUntil(std::chrono::time_point<std::chrono::system_clock> &&in_time) {
	return false;
}
bool handleRunLoopMsg(UINT msg) {
	if (msg == WM_USER_RunLoopMsg)
	{
		gXMainRunloop->_do();
		return true;
	}
	else {
		return false;
	}
}