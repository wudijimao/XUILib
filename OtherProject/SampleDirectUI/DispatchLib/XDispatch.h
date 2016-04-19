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
	typedef enum XTaskPriority {
		XTaskPriority_High,
		XTaskPriority_Default,
		XTaskPriority_Low,
		XTaskPriority_Background,
	}XTaskPriority;

	class XThreadPool;
	//TODO::换用无锁队列（lock-free）、提高效率
	class XTaskQueue {
		std::mutex mutex;
		std::queue<MyFun*> queue;
	public:
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
		//初始化两个全局Pool
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
		inline MyFun* copyFunction(const MyFun &fun);
	public:
		static XDispatchManager* getSharedInstance();
		XDispatchManager();
		void dispatchAsnyc(std::shared_ptr<XTaskQueue> taskQueue, const MyFun &fun);
		void dispatchAfter(std::shared_ptr<XTaskQueue> &taskQueue, const MyFun &fun, long delayMS);
	};

	inline void XDispatchManager::dispatchAsnyc(std::shared_ptr<XTaskQueue> taskQueue, const MyFun &fun) {
		taskQueue->push(copyFunction(fun));
		if (taskQueue->runInPool) {
			taskQueue->runInPool->onQueueChanged();
		}
	}
}