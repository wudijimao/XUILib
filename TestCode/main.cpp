#include <XDUILib.hpp>

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

class TestTextView : public XUI::UIView {
    virtual void drawRect(IXRender &render) {
        XResource::XRect rect = getFixRect();
        rect.moveX(10.0).moveY(10.0).increaceWidth(-20.0).increaceHeight(-20.0);
       // render.DrawBackGround(XResource::XUIColor::uiColor(255, 255, 0, 255)->_color , rect);
        render.DrawString("HELLOWWORLD!", rect);
    }
};

class ViewController : public XUI::UIViewController {
public:
	std::shared_ptr<XUI::UIView> _testSubView;
	virtual void viewDidLoad() override {
		XUI::UIViewController::viewDidLoad();
		_testSubView = std::make_shared<XUI::UIView>();
		_testSubView->setBkgColor(XResource::XUIColor::redColor());
		_testSubView->setRect(XResource::XRectPro(20, 20, 320, 180));
		this->view()->addSubView(_testSubView);
		auto subView2 = std::make_shared<TestTextView>();
        auto color = XResource::XUIColor::pinkColor()->copy();
        color->_color.a = 200;
		subView2->setBkgColor(color);
		subView2->setRect(XResource::XRectPro(20, 120, 100, 320));
		this->view()->addSubView(subView2);
		auto subView3 = std::make_shared<XUI::UIView>();
		subView3->setBkgColor(XResource::XUIColor::blueColor());
		subView3->setRect(XResource::XRectPro(5, 20, 15, 50));
		_testSubView->addSubView(subView3);
		auto img = XResource::XImage::imageNamed("test.png");
		subView2->setBkgImg(img);
		auto request = std::make_shared<XHTTPRequest>();
		request->url = "http://ww1.sinaimg.cn/mw690/4e3af31cjw1f3gtjp10lhj20ci071t8w.jpg";
		request->finishCallBack = [&](std::shared_ptr<XHTTPResponse> response) {
            auto image = std::make_shared<XResource::XImage>(response->data());
            _testSubView->setBkgImg(image);
		};
		IXHTTPClient::getSharedInstanc()->sendRequest(request);
		
	}
	virtual void onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) override {
		(*touch.begin())->_belongView->setBkgColor(XResource::XUIColor::greenColor());
	}
	virtual void onMouseEvent(const std::vector<std::shared_ptr<XMouse>> &mouseEvents) override {
		auto event = (*mouseEvents.begin());
		if (event->eventType == MouseEventType::Up)
		{
			if (event->eventButton == MouseEventButton::Left) {
				event->_belongView->setBkgColor(XResource::XUIColor::grayColor());
			} else {
				event->_belongView->setBkgColor(XResource::XUIColor::redColor());
			}
		}
	}
};

class XTestApp : public XApp {
public:
	virtual bool init() {
		setMainWindow(IXWindow::createWindow());
		auto vc = std::make_shared<ViewController>();
		mainWindow()->setRootViewController(vc);
		return true;
	}
};


XDUIMain {
	DetectMemoryLeaks();
	auto thisApp = XTestApp();
	thisApp.run(XDUI_RUN_PARAMS);
}