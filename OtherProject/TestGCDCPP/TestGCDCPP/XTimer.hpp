//
//  XTimer.hpp
//  TestGCDCPP
//
//  Created by ximiao on 16/3/3.
//  Copyright © 2016年 ximiao. All rights reserved.
//  上层的 用户可创建的类型全部写成返回share_ptr  并且自己持有一个weak_ptr  self的形式
//
#pragma once
#include "XRunLoop.hpp"
#include <functional>
#include "XDispatch.h"

enum class XTimmerStatus {
    Start,
    Stop,
    Pause,
    Resume,
    Dealloc,
};

//final
class XTimer {
    class XTimmerDelegate {
    public:
        virtual void onTimmerStatusChange(XTimer &timmer, XTimmerStatus status){};
        virtual void onTimmerFired(XTimer &timmer) = 0;
    };
public:
    std::shared_ptr<XTimer> createInstance() {
        auto timmer = new XTimer();
        auto timmerPtr = std::shared_ptr<XTimer>(timmer);
        timmer->_self = timmerPtr;
        return timmerPtr;
    }
    ~XTimer() {
        auto delegate = _delegate.lock();
        if (delegate) {
            delegate->onTimmerStatusChange(*this, XTimmerStatus::Dealloc);
        }
    }
    bool start() {
        auto runloop = getCurrentRunLoop();
        auto delegate = _delegate.lock();
        if (delegate) {
            delegate->onTimmerStatusChange(*this, XTimmerStatus::Start);
        }
        return true;
    }
    void stop() {
        
    }
    void pause() {
        
    }
    void resume() {
        
    }
    //Must keep func dependence exist, befroe clearFireFun or remove timmer
    void setFireFun(std::function<void(XTimer &timmer)> &func) {
        _fireFun.reset(new std::function<void(XTimer &timmer)>(func));
    }
    void clearFireFun() {
        _fireFun.reset();
    }
    std::shared_ptr<XTimmerDelegate> getDelegate() {
        return _delegate.lock();
    }
    void setDelegate(std::shared_ptr<XTimmerDelegate> &delegate) {
        _delegate = delegate;
    }
    
protected:
    std::weak_ptr<XTimer> _self;
private:
    XTimer() {
    }
    void fire() {
        auto delegate = _delegate.lock();
        if (delegate) {
            delegate->onTimmerFired(*this);
        }
        if (_fireFun) {
            (*_fireFun)(*this);
        }
    }
    void callStatusChange(XTimmerStatus status) {
        XDispatch::dispatchAsnyc(XDispatch::getMainQueue(), ^(){
            auto self = _self.lock();
            if (self) {
                auto delegate = _delegate.lock();
                if (delegate) {
                   delegate->onTimmerStatusChange(*this, status);
                }
            }
        });
    }
    std::shared_ptr<std::function<void(XTimer &timmer)>> _fireFun;
    std::weak_ptr<XTimmerDelegate> _delegate;
};


