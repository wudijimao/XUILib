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
    XUI::UIViewController::viewDidLoad();
    
    view()->setBkgImg(XResource::XImage::imageNamed("login_bkg.png"));
    auto title = std::make_shared<XUI::UITextView>();
    auto rect = XResource::XRectPro(0, 100, view()->getRect().size().Width(), 50);
    title->setRect(rect);
    title->setText("WeChat"); //WeTalk
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
    
    
    auto btn = std::make_shared<XUI::XButton>();
    rect.moveY(120);
    btn->setRect(XResource::XRect(200,300,100,100));
    btn->setTitle("abc");
    view()->addSubView(btn);
    
    /*addAnimation(XUI::ValueAnimation<int>::createAni(0, 100, [nameText](int a) {
     auto rect = nameText->getRect();
     rect.X(a);
     nameText->setRect(rect);
    })).setRepeatTimes(10000).play();*/
}



