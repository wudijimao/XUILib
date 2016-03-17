#pragma once

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <atomic>
#include "XTask.hpp"

namespace XDispatch {
//    class TestMyFun {
//    public:
//        TestMyFun() {
//            
//        }
//        ~TestMyFun() {
//            
//        }
//        void operator() (void) {
//            int b = 0;
//            for (int i = 0; i < 10000; ++i) {
//                b += 2;
//            }
//        }
//    };
//    typedef TestMyFun MyFun;


	class XThreadPool {
	private:
		std::atomic_int_fast8_t threadNum;
		int_fast8_t maxNum;
		std::atomic_int taskNum;
		std::mutex mutex;
		std::condition_variable cv;
		void runLoop();
	public:
		std::vector<std::shared_ptr<XTaskQueue>> taskQueue;
		void onQueueChanged();
		//≥ı ºªØ¡Ω∏ˆ»´æ÷Pool
		static void initGlobelPool();
		void startNewThread();
		XThreadPool();
		virtual ~XThreadPool();
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
		static XDispatchManager* getSharedInstance();
		XDispatchManager();
        ~XDispatchManager();
		inline void dispatchAsnyc(std::shared_ptr<XTaskQueue> taskQueue, const MyFun &fun) {
			taskQueue->push(copyFunction(fun));
			if (taskQueue->runInPool) {
				taskQueue->runInPool->onQueueChanged();
			}
		}
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
        return XTaskQueue::getMainQueue();
    }
}

