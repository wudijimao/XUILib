//
//  LoginViewController.cpp
//  TestUI
//
//  Created by ximiao on 16/5/14.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma execution_character_set("utf-8")

#include "LoginViewController.hpp"
#include "ChatClient.hpp"


void LoginViewController::viewDidLoad() {
    XUI::UIViewController::viewDidLoad();
    
    
    view()->setBkgImg(XResource::XImage::imageNamed("login_bkg.png"));
    auto title = std::make_shared<XUI::UITextView>();
    auto rect = XResource::XRectPro(0, 70, view()->getRect().size().Width(), 30);
    title->setRect(rect);
    title->setText("WeChat"); //WeTalk
    std::shared_ptr<XResource::XFont> font = XResource::XFont::font("FZMiaoWuS-GB", 60);
    title->setFont(font);
    title->setBkgColor(XResource::XUIColor::clearColor());
    title->setTextColor(XResource::XUIColor::whiteColor());
    title->setAlignmentH(XUI::UITextAlignmentH::Center);
    title->setAlignmentV(XUI::UITextAlignmentV::Center);
    view()->addSubView(title);
    
    _nameText = std::make_shared<XUI::UIEditView>();
    _nameText->setBkgColor(XResource::XUIColor::clearColor());
    _nameText->setBkgImg(XResource::XImage::imageNamed("login_edit.png")->createStretchableImage(5, 5));
    rect.Width(220);
    rect.moveY(50);
    rect.HAlign(XResource::XRectPro::HAlignType::HAlign_Center);
    _nameText->setRect(rect);
    view()->addSubView(_nameText);
    
    
    auto btn = std::make_shared<XUI::XButton>();
    rect.moveX(-100);
    rect.moveY(50);
    rect.Width(60);
    rect.Height(30);
    btn->setRect(rect);
    btn->setTitle("login");
    btn->onClick = [&](XUI::XButton&) {
        ChatClient &client = ChatClient::getInstance();
        client.mUserID = _nameText->text()->getString().getUTF8String();
        client.run();
    };
    view()->addSubView(btn);
    btn = std::make_shared<XUI::XButton>();
    rect.moveX(120);
    btn->setRect(rect);
    btn->setTitle("sendg");
    btn->onClick = [&](XUI::XButton&) {
        ChatClient::getInstance().sendSimpleText(_nameText->text()->getString().UTF8CStr());
    };
    view()->addSubView(btn);
    rect.moveX(-120);
    _chatText = std::make_shared<XUI::UITextView>();
    rect.moveY(60);
    rect.Width(300);
    rect.Height(400);
    _chatText->setRect(rect);
    view()->addSubView(_chatText);
    
    ChatClient::getInstance().onLoginFinish = [&](ChatClient&c, std::error_code e) {
        _chatText->setText("登陆成功");
    };
    ChatClient::getInstance().onRecvSimpleText = [&](ChatClient&c, const char* str) {
        auto text = _chatText->text()->getString();
        _chatText->setText(text + "    " + str);
    };
    
    /*addAnimation(XUI::ValueAnimation<int>::createAni(0, 100, [nameText](int a) {
     auto rect = nameText->getRect();
     rect.X(a);
     nameText->setRect(rect);
    })).setRepeatTimes(10000).play();*/
}



