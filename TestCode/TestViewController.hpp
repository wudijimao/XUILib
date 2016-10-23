//
//  TestViewController.hpp
//  TestUI
//
//  Created by ximiao on 16/6/24.
//  Copyright © 2016年 wudijimao. All rights reserved.
//
#pragma once
#pragma execution_character_set("utf-8")

#include <XDUILib.hpp>


class ViewController : public XUI::UIViewController {
public:
    long long mNum = 0;
    ~ViewController();
    std::shared_ptr<XUI::XView> _testSubView;
    virtual void viewDidLoad() override;
    virtual void onTouch(const std::vector<std::shared_ptr<XTouch>> &touch) override;
    virtual void onMouseEvent(const std::vector<std::shared_ptr<XMouse>> &mouseEvents) override;
};
