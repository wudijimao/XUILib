#include <XDUILib.hpp>
#pragma execution_character_set("utf-8")
#include "LoginViewController.hpp"
#include "TestViewController.hpp"

//  ****Important:please set main.cpp to Object-C++ Source Type in Xcode at the FilePropty; see Xcode Setting.png in Doc******

using namespace XDUILib;


void DetectMemoryLeaks()
{
	// Enable the memory leak tracking tools built into the CRT runtime.

#if defined(_DEBUG)
	//_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif
}



class XTestApp : public XApp {
public:
    std::shared_ptr<XUI::UIViewController> mVc;
	virtual bool init() {
		__super::init();
		setMainWindow(IXWindow::createWindow());
		mVc = std::make_shared<ViewController>();
		mainWindow()->setRootViewController(mVc);
		return true;
	}
private:
#ifndef _WINDOWS
    typedef XApp __super;
#endif
};

XDUIMain {
	//DetectMemoryLeaks();
#ifdef TARGET_OS_ANDROID
	app_dummy();
	keepJNIBridgeSampleInApk();
#endif
	auto thisApp = new XTestApp();
	thisApp->run(XDUI_RUN_PARAMS);
	delete thisApp;
}