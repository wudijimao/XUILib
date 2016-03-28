//
//  main.m
//  TestUI
//
//  Created by ximiao on 16/3/17.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#import <UIKit/UIKit.h>
#include <XDUILib.hpp>


using namespace XDUILib;

class ViewController : public XUI::UIViewController {
public:
    virtual void viewDidLoad() {
        XUI::UIViewController ::viewDidLoad();
        auto testSubView = std::make_shared<XUI::UIView>();
        testSubView->setBkgColor(XResource::XUIColor::redColor());
        testSubView->setRect(XResource::XRectPro(20,20, 150,150));
        this->getView()->addSubView(testSubView);
        auto subView2 = std::make_shared<XUI::UIView>();
        subView2->setBkgColor(XResource::XUIColor::pinkColor());
        subView2->setRect(XResource::XRectPro(20,120, 100,320));
        this->getView()->addSubView(subView2);
        auto subView3 = std::make_shared<XUI::UIView>();
        subView3->setBkgColor(XResource::XUIColor::blueColor());
        subView3->setRect(XResource::XRectPro(5,20, 15,50));
        testSubView->addSubView(subView3);
    }
};

class XTestApp : public XApp {
public:
    virtual bool init() {
        setMainWindow(std::make_shared<XWindow>());
        auto vc = std::make_shared<ViewController>();
        mainWindow()->setRootViewController(vc);
        return true;
    }
};


int main(int argc, char * argv[]) {
    auto thisApp = XTestApp();
    thisApp.run(argc, argv);
}
