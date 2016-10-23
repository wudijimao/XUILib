//
// Created by ximiao1 on 16/10/19.
//
#pragma once

#include "UIViewController.hpp"

namespace XUI {

    class SIMPLEDIRECTUI_API XNavigationManager {
    private:
        std::vector<std::shared_ptr<UIViewController>> mViewControllers;
        std::shared_ptr<UIViewController> mTopVC;

    public:
        const std::vector<std::shared_ptr<UIViewController>> getAll();
        const std::shared_ptr<UIViewController>& getTop();
        std::shared_ptr<UIViewController> pop();
        void pop(const UIViewController*);
        void push(const std::shared_ptr<UIViewController>&);
        void remove(const UIViewController*);
        void insert(const std::shared_ptr<UIViewController>&, int index);
        void insertFromEnd(const std::shared_ptr<UIViewController>&, int rIndex);
        void insert(const std::shared_ptr<UIViewController>& insert, const UIViewController *at, bool atFront = true);
        void replace(const std::shared_ptr<UIViewController>& insert, const UIViewController *at);
        void clear();
        void resetRoot(const std::shared_ptr<UIViewController>&);
    };
}