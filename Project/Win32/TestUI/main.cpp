#include <XDUILib.hpp>


using namespace XDUILib;

class ViewController : public XUI::UIViewController {
public:
	std::shared_ptr<XUI::UIView> _testSubView;
	virtual void viewDidLoad() {
		XUI::UIViewController::viewDidLoad();
		_testSubView = std::make_shared<XUI::UIView>();
		_testSubView->setBkgColor(XResource::XUIColor::redColor());
		_testSubView->setRect(XResource::XRectPro(20, 20, 320, 180));
		this->getView()->addSubView(_testSubView);
		auto subView2 = std::make_shared<XUI::UIView>();
		subView2->setBkgColor(XResource::XUIColor::pinkColor());
		subView2->setRect(XResource::XRectPro(20, 120, 100, 320));
		this->getView()->addSubView(subView2);
		auto subView3 = std::make_shared<XUI::UIView>();
		subView3->setBkgColor(XResource::XUIColor::blueColor());
		subView3->setRect(XResource::XRectPro(5, 20, 15, 50));
		_testSubView->addSubView(subView3);
		auto img = std::make_shared<XResource::XImage>("/Users/ximiao/GCD-dispatch-/Project/iOS/TestUI/TestUI/test.jpg");
		_testSubView->setBkgImg(img);
	}
	virtual void onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) override {
		(*touch.begin())->_belongView->setBkgColor(XResource::XUIColor::greenColor());
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


int main(int argc, char * argv[]) {
	auto thisApp = XTestApp();
	thisApp.run(argc, argv);
}