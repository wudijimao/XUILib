#pragma once

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <atomic>

namespace XDispatch {
	typedef std::function<void(void)> MyFun;
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
    
	typedef enum XTaskPriority {
		XTaskPriority_High,
		XTaskPriority_Default,
		XTaskPriority_Low,
		XTaskPriority_Background,
	}XTaskPriority;

	class XThreadPool;
	//TODO::ªª”√ŒﬁÀ¯∂”¡–£®lock-free£©°¢Ã·∏ﬂ–ß¬ 
	class XTaskQueue {
		std::mutex mutex;
		std::queue<MyFun*> queue;
	public:
        ~XTaskQueue();
		XThreadPool *runInPool;
		static std::shared_ptr<XTaskQueue> getMainQueue();
		static std::shared_ptr<XTaskQueue> getGlobleQueue(XTaskPriority priority);
		bool pop(MyFun *&fun);
		void push(MyFun *fun);
	};


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

	struct XTask {
	public:
		XTask(MyFun *in_fun,
			std::shared_ptr<XTaskQueue> &in_queue,
			std::chrono::time_point<std::chrono::system_clock> &&in_time);
        ~XTask();
		MyFun *fun;
		std::shared_ptr<XTaskQueue> queue;
		std::chrono::time_point<std::chrono::system_clock> time;
		bool operator <(const XTask& rh);
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
}