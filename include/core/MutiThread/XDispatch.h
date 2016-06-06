#pragma once

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <atomic>
#include "XTask.hpp"
#include "XRunLoop.hpp"
#include "XTaskQueue.hpp"

namespace XDispatch {
    
    class XRunLoopDispatchSource : public XRunLoopSource {
    public:
        virtual void _do() override {
            MyFun *fun = NULL;
            while (mTaskQueue->pop(fun)) {
                (*fun)();
                delete fun;
            }
        }
    public:
        std::shared_ptr<XTaskQueue> mTaskQueue;
    };

	class XDispatchManager {
	private:
		std::mutex mutex;
		std::condition_variable cv;
		std::vector<XTask*> mQueue;
		std::vector<XTask*> mUnSortVector;
		void runLoop();
		MyFun* copyFunction(const MyFun &fun);
	public:
        static std::shared_ptr<XRunLoopDispatchSource> mMainThreadSource;
		static XDispatchManager* getSharedInstance();
		XDispatchManager();
        ~XDispatchManager();
        void dispatchAsnyc(std::shared_ptr<XTaskQueue> taskQueue, const MyFun &fun);
		void dispatchAfter(std::shared_ptr<XTaskQueue> taskQueue, const MyFun &fun, long delayMS);
	};
    
    inline void dispatchAsnyc(std::shared_ptr<XTaskQueue> taskQueue, const MyFun &fun) {
        XDispatchManager::getSharedInstance()->dispatchAsnyc(taskQueue, fun);
    }
    inline void dispatchAfter(std::shared_ptr<XTaskQueue> taskQueue, const MyFun &fun, long delayMS) {
        XDispatchManager::getSharedInstance()->dispatchAfter(taskQueue, fun, delayMS);
    }
    inline std::shared_ptr<XTaskQueue> getMainQueue() {
        return XTaskQueue::getMainQueue();
    }
    inline std::shared_ptr<XTaskQueue> getGlobleQueue(XTaskPriority priority) {
        return XTaskQueue::getGlobleQueue(priority);
    }
}

