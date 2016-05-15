//
//  XTimer.hpp
//  TestGCDCPP
//
//  Created by ximiao on 16/5/15.
//  Copyright © 2016年 ximiao. All rights reserved.
//
#pragma once
#include "../stdafx.hpp"
#include "XRunLoop.hpp"
#include "XDispatch.h"

namespace XDispatch {
    struct SIMPLEDIRECTUI_API XTimer {
    public:
		static std::shared_ptr<XTimer> timer(unsigned long intervalMS) {
			std::shared_ptr<XTimer> t;
			t.reset(new XTimer());
			t->setIntervalMS(intervalMS);
			t->mSelf = t;
			return t;
		}
		bool start() {
			auto runLoop = getCurrentRunLoop();
			return startInRunloop(runLoop);
		}
		bool startInRunloop(std::shared_ptr<IXRunLoop> runloop) {
			if (mTickCallBackFun)
			{
				mIsRunning = true;
				mRunloop = runloop;
				auto a1 = mRunloop.get();
				auto a2 = getMainRunLoop().get();
				if (a1 == a2)
				{
					processInMainRunloop();
				}
				return true;
			}
			return false;
		}
		void stop() {
			mIsRunning = false;
		}
		void setIntervalMS(unsigned long intervalMS) {
			mIntervalMS = intervalMS;
		}
		unsigned long intervalMS() {
			return mIntervalMS;
		}
		XTimer& setTickFun(const std::function<void(const std::shared_ptr<XTimer>&)> &fun) {
			mTickCallBackFun = fun;
			return *this;
		}
	protected:
		std::weak_ptr<XTimer> mSelf;
		XTimer() {};
	private:
		void processInMainRunloop() {
			std::shared_ptr<XTimer> self = mSelf.lock();
			dispatchAfter(getMainQueue(), [self]() {
				if (self->mIsRunning && self->mTickCallBackFun)
				{
					self->mTickCallBackFun(self);
					self->processInMainRunloop();
				}
			}, mIntervalMS);
		}
		std::function<void(const std::shared_ptr<XTimer>&)> mTickCallBackFun;
		std::shared_ptr<IXRunLoop> mRunloop;
		unsigned long mIntervalMS;
		bool mIsRunning = false;
    };
}
