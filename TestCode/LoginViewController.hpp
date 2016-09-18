//
//  LoginViewController.hpp
//  TestUI
//
//  Created by ximiao on 16/5/14.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#pragma once
#include <sstream>
#include <chrono>

#include <XDUILib.hpp>
#pragma execution_character_set("utf-8")



class LoginViewController : public XUI::UIViewController {
    virtual void viewDidLoad() override;
private:
    std::shared_ptr<XUI::UIEditView> _nameText;
    std::shared_ptr<XUI::UITextView> _chatText;
};
