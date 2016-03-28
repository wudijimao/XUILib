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

class XTestApp : public XApp {
public:
    static std::shared_ptr<XTestApp> thisApp() {
        static std::shared_ptr<XTestApp> app = std::make_shared<XTestApp>();
        return app;
    }
};


int main(int argc, char * argv[]) {
    auto thisApp = XTestApp::thisApp();
    thisApp->run(argc, argv);
}
