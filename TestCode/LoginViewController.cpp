//
//  LoginViewController.cpp
//  TestUI
//
//  Created by ximiao on 16/5/14.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma execution_character_set("utf-8")

#include "LoginViewController.hpp"


void LoginViewController::viewDidLoad() {
    view()->setBkgImg(XResource::XImage::imageNamed("login_bkg.png"));
    auto title = std::make_shared<XUI::UITextView>();
    auto rect = XResource::XRectPro(0, 100, view()->getRect().size().Width(), 50);
    title->setRect(rect);
    title->setText("WeTalk");
    title->setFont(XResource::XFont::font("FZMiaoWuS-GB", 60));
    title->setBkgColor(XResource::XUIColor::clearColor());
    title->setTextColor(XResource::XUIColor::whiteColor());
    title->setAlignmentH(XUI::UITextAlignmentH::Center);
    title->setAlignmentV(XUI::UITextAlignmentV::Center);
    view()->addSubView(title);
    
    auto nameText = std::make_shared<XUI::UIEditView>();
    nameText->setBkgColor(XResource::XUIColor::clearColor());
    nameText->setBkgImg(XResource::XImage::imageNamed("login_edit.png")->createStretchableImage(5, 5));
    rect.Width(220);
    rect.moveY(120);
    rect.HAlign(XResource::XRectPro::HAlignType::HAlign_Center);
    nameText->setRect(rect);
    view()->addSubView(nameText);
    
    addAnimation(XUI::ValueAnimation<int>::createAni(0, 100, [nameText](int a) {
     auto rect = nameText->getRect();
     rect.X(a);
     nameText->setRect(rect);
    })).setRepeatTimes(10000).play();
    
    auto testView = std::make_shared<XUI::UIView>();
    testView->setBkgColor(XResource::XUIColor::greenColor());
    testView->setRect(XResource::XRectPro(100,400,100,100));
    testView->setMaskImg(XResource::XImage::imageNamed("test_mask.png"));
    testView->setBkgImg(XResource::XImage::imageNamed("test.png"));
    testView->setClipsToBounds(true);
    //view()->addSubView(testView);
    
    auto testView2 = std::make_shared<XUI::UIView>();
    testView2->setBkgImg(XResource::XImage::imageNamed("test.jpg"));
    testView2->setRect(XResource::XRectPro(50,50,100,100));
    testView->addSubView(testView2);
    
    auto request = std::make_shared<XHTTPRequest>();
    request->url = "http://www.baidu.com";
    request->_header.setUserAgent("aaaa");
    request->finishCallBack = [&](std::shared_ptr<XHTTPResponse> response) {
        int a = 0;
    };
    IXHTTPClient::getSharedInstanc()->sendRequest(request);
}



