//
// Created by ximiao1 on 16/10/19.
//
#pragma once

#include "UIViewController.hpp"

namespace XUI {

    class XNavagationManager {
        std::vector<std::shared_ptr<UIViewController>> mViewController;
    };
}