#include <XDUILib.hpp>
#pragma execution_character_set("utf-8")
#include "LoginViewController.hpp"


#include <dispatch/dispatch.h>

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
    long long mNum = 0;
    ~ViewController() {
        
    }
	std::shared_ptr<XUI::UIView> _testSubView;
	virtual void viewDidLoad() override {
		XUI::UIViewController::viewDidLoad();
        
        auto jpgImg = XResource::XImage::imageNamed("test.jpg");
        
		_testSubView = std::make_shared<XUI::UIView>();
		_testSubView->setBkgColor(XResource::XUIColor::redColor());
		_testSubView->setRect(XResource::XRectPro(20, 20, 320, 120));
        _testSubView->setBkgImg(XResource::XImage::imageNamed("test.jpg"));
		this->view()->addSubView(_testSubView);
        
        
        auto scrollView = std::make_shared<XUI::ScrollView>();
        scrollView->setBkgColor(XResource::XUIColor::pinkColor());
        scrollView->setRect(XResource::XRectPro(20, 100, 100, 200));
        this->view()->addSubView(scrollView);
        
        auto textView = std::make_shared<XUI::UITextView>();
        //textView->setText("泉此方测试测试 test哈哈哈啊啊啊");
        textView->setTextColor(XResource::XUIColor::blueColor());
        textView->setFont(XResource::XFont::systemFont(16));
        auto color = XResource::XUIColor::pinkColor()->copy();
        color->_color.a = 200;
		textView->setBkgColor(color);
		textView->setRect(XResource::XRectPro(0, 0, 100, 320));
        auto img = XResource::XImage::imageNamed("test.png");
        textView->setBkgImg(img);
        //scrollView->setContentView(textView);
        
		auto btn = std::make_shared<XUI::XButton>();
        btn->setBackgroundColor(XUI::ButtonStates::DOWN, XResource::XUIColor::blueColor());
		btn->setRect(XResource::XRectPro(5, 20, 70, 25));
		//_testSubView->addSubView(btn);
		
		auto request = std::make_shared<XHTTPRequest>();
		request->url = "http://ww3.sinaimg.cn/mw690/82d67d7bjw1f3iji5sydzj21kw0zkgt6.jpg";
		request->finishCallBack = [&](std::shared_ptr<XHTTPResponse> response) {
            auto image = XResource::XImage::imageFromData(response->data());
            //_testSubView->setBkgImg(image);
            //this->view()->addSubView(_testSubView);
		};
		//IXHTTPClient::getSharedInstanc()->sendRequest(request);
        
        auto imgView = std::make_shared<XUI::XUIImageView>();
        imgView->setRect(XResource::XRectPro(100, 300, 200, 200));
        imgView->setImage(jpgImg);
        this->view()->addSubView(imgView);
        imgView->setScaleType(XUI::XUIImageScaleType::AspectFill);
        imgView->setBkgColor(XResource::XUIColor::grayColor());
        
        //auto vc = std::make_shared<LoginViewController>();
        //this->presentViewControler(vc);
//        clock_t t1 = clock();
//        for (int i = 0; i < 4; ++i) {
//            XDispatch::dispatchAsnyc(XDispatch::getGlobleQueue(XDispatch::XTaskPriority::XTaskPriority_Default), [this, i, t1](){
//                for (int j = 0; j < 100000; ++j) {
//                    XDispatch::dispatchAfter(XDispatch::getMainQueue(), [this, i, j, t1](){
//                        ++mNum;
//                        XLog::sharedInstance().log(XLogLevel::Debug, "%d:%d___%d\r\n", i, j, mNum);
//                        if (mNum == 400000) {
//                            clock_t t2 = clock();
//                            std::cout << "count:" << mNum << "   time:" << (t2 - t1) / (CLOCKS_PER_SEC / 1000.0) << "ms" << std::endl;
//                        }
//                    }, 100);
//                }
//            });
//        }
        
//        for (int i = 0; i < 400000; ++i) {
//            ++mNum;
//            XLog::sharedInstance().log(XLogLevel::Debug, "%d:%d___%d\r\n", i, i, mNum);
//            if (mNum == 400000) {
//                clock_t t2 = clock();
//                std::cout << "count:" << mNum << "   time:" << (t2 - t1) / (CLOCKS_PER_SEC / 1000.0) << "ms" << std::endl;
//            }
//        }

//        __block ViewController *bThis = this;
//        for (int i = 0; i < 4; ++i) {
//            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
//                for (int j = 0; j < 100000; ++j) {
//                    //XLog::sharedInstance().log(XLogLevel::Debug, "%d:%d\r\n", i, j);
//                    void(bl)(void) = ^{
//                        ++bThis->mNum;
//                        //XLog::sharedInstance().log(XLogLevel::Debug, "%d:%d___%d\r\n", i, j, bThis->mNum);
//                        if (bThis->mNum == 400000) {
//                            clock_t t2 = clock();
//                            std::cout << "count:" << mNum << "   time:" << (t2 - t1) / (CLOCKS_PER_SEC / 1000.0) << "ms" << std::endl;
//                        }
//                    }
//                    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(100 * NSEC_PER_MSEC)), dispatch_get_main_queue(), );
//                }
//            });
//        }
        
        
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
    std::shared_ptr<ViewController> mVc;
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
	DetectMemoryLeaks();
	auto thisApp = XTestApp();
	thisApp.run(XDUI_RUN_PARAMS);
}