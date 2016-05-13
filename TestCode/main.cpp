#include <XDUILib.hpp>
#pragma execution_character_set("utf-8")



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

class ViewController : public XUI::UIViewController {
public:
	std::shared_ptr<XUI::UIView> _testSubView;
	virtual void viewDidLoad() override {
		XUI::UIViewController::viewDidLoad();
		_testSubView = std::make_shared<XUI::UIView>();
		_testSubView->setBkgColor(XResource::XUIColor::redColor());
		_testSubView->setRect(XResource::XRectPro(20, 20, 320, 180));
		this->view()->addSubView(_testSubView);
        
        auto textView = std::make_shared<XUI::UITextView>();
        textView->setText("泉此方测试测试 test哈哈哈啊啊啊");
        textView->setTextColor(XResource::XUIColor::blueColor());
        textView->setFont(XResource::XFont::systemFont(16));
        auto color = XResource::XUIColor::pinkColor()->copy();
        color->_color.a = 200;
		textView->setBkgColor(color);
		textView->setRect(XResource::XRectPro(20, 120, 100, 320));
        auto img = XResource::XImage::imageNamed("test.png");
        textView->setBkgImg(img);
		this->view()->addSubView(textView);
        
		auto btn = std::make_shared<XUI::XButton>();
        btn->setBackgroundColor(XUI::ButtonStates::DOWN, XResource::XUIColor::blueColor());
		btn->setRect(XResource::XRectPro(5, 20, 70, 25));
		_testSubView->addSubView(btn);
		
		auto request = std::make_shared<XHTTPRequest>();
		request->url = "http://ww3.sinaimg.cn/mw690/82d67d7bjw1f3iji5sydzj21kw0zkgt6.jpg";
		request->finishCallBack = [&](std::shared_ptr<XHTTPResponse> response) {
            auto image = XResource::XImage::imageFromData(response->data());
            _testSubView->setBkgImg(image);
		};
		IXHTTPClient::getSharedInstanc()->sendRequest(request);
        
        auto imgView = std::make_shared<XUI::XUIImageView>();
        imgView->setRect(XResource::XRectPro(100, 300, 200, 200));
        imgView->setImage(XResource::XImage::imageNamed("test.jpg"));
        this->view()->addSubView(imgView);
        imgView->setScaleType(XUI::XUIImageScaleType::AspectFill);
        imgView->setBkgColor(XResource::XUIColor::grayColor());
	}
	virtual void onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) override {
		//(*touch.begin())->_belongView->setBkgColor(XResource::XUIColor::greenColor());
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