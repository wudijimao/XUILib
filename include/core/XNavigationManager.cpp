//
// Created by ximiao1 on 16/10/19.
//

#include "XNavigationManager.hpp"


namespace XUI {

    const std::shared_ptr<UIViewController>& XNavigationManager::getTop() {
        return mTopVC;
    }

    std::shared_ptr<UIViewController> XNavigationManager::pop() {
        if (mTopVC) {
            auto ret = mTopVC;
            mViewControllers.pop_back();
            if (mViewControllers.size() > 0) {
                mTopVC = mViewControllers.back();
            } else {
                mTopVC.reset();
            }
            return ret;
        }
        return mTopVC;
    }

    void XNavigationManager::pop(const UIViewController* vc) {

    }

    void XNavigationManager::push(const std::shared_ptr<UIViewController> &vc) {
        mTopVC = vc;
        mViewControllers.push_back(vc);
    }

    void XNavigationManager::remove(const UIViewController*) {

    }

    void XNavigationManager::insert(const std::shared_ptr<UIViewController> &, int index) {

    }

    void XNavigationManager::insertFromEnd(const std::shared_ptr<UIViewController> &, int rIndex) {

    }

    void XNavigationManager::insert(const std::shared_ptr<UIViewController> &insert, const UIViewController *at,
                bool atFront) {

    }
    void XNavigationManager::replace(const std::shared_ptr<UIViewController>& insert, const UIViewController *at) {

    }
    void XNavigationManager::clear() {
        mViewControllers.clear();
    }
    void XNavigationManager::resetRoot(const std::shared_ptr<UIViewController>&vc) {
        mViewControllers.clear();
        mViewControllers.push_back(vc);
        mTopVC = vc;
    }
}