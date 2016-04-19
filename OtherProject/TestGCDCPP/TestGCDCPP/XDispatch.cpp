#include "XDispatch.h"
#include "XRunLoop.hpp"

namespace XDispatch {
    class XRunLoopDispatchSource : public XRunLoopSource {
    public:
        void _do() {
            MyFun *fun = NULL;
            while (mTaskQueue->pop(fun)) {
                (*fun)();
                delete fun;
            }
        }
    public:
        std::shared_ptr<XTaskQueue> mTaskQueue;
    };
    std::shared_ptr<XRunLoopDispatchSource> source;

	std::shared_ptr<XTaskQueue> XTaskQueue::getMainQueue() {
		//TODO::maybe mutithread cause problem
		static std::shared_ptr<XTaskQueue> mainQueue(new XTaskQueue());
		return mainQueue;
	}
	std::shared_ptr<XTaskQueue> XTaskQueue::getGlobleQueue(XTaskPriority priority) {
		static std::shared_ptr<XTaskQueue> backgroundQueue(new XTaskQueue());
		static std::shared_ptr<XTaskQueue> lowQueue(new XTaskQueue());
		static std::shared_ptr<XTaskQueue> defaultQueue(new XTaskQueue());
		static std::shared_ptr<XTaskQueue> highQueue(new XTaskQueue());
		switch (priority) {
		case XTaskPriority_Background:
			return backgroundQueue;
		case XTaskPriority_Low:
			return lowQueue;
		case XTaskPriority_High:
			return highQueue;
		case XTaskPriority_Default:
		default:
			return defaultQueue;
		}
	}
	bool XTaskQueue::pop(MyFun *&fun) {
		std::unique_lock<std::mutex> lk(mutex);
		if (queue.size() > 0) {
			fun = queue.front();
			queue.pop();
			return true;
		}
		return false;
	}
	void XTaskQueue::push(MyFun *fun) {
		mutex.lock();
		queue.push(fun);
		mutex.unlock();
	}
    XTaskQueue::~XTaskQueue() {
        
    }


	void XThreadPool::runLoop() {
		++threadNum;
		do {
			std::unique_lock<std::mutex> lk(mutex);
			auto iter = taskQueue.begin();
			auto end = taskQueue.end();
			MyFun *fun = NULL;
			bool hasFun = false;
			while (iter != end) {
				if ((*iter)->pop(fun)) {
					hasFun = true;
					break;
				}
				++iter;
			}
			if (hasFun) {
				lk.unlock();
				(*fun)();
				delete fun;
			}
			else {
				cv.wait(lk);
			}
		} while (1);
		--threadNum;
	}
	void XThreadPool::onQueueChanged() {
		mutex.lock();
		cv.notify_one();
		mutex.unlock();
	}
	void XThreadPool::initGlobelPool() {
//		static XThreadPool mainThread;
//		mainThread.taskQueue.push_back(XTaskQueue::getMainQueue());
//		XTaskQueue::getMainQueue()->runInPool = &mainThread;
//		mainThread.startNewThread();
        
		static XThreadPool backgroundThreadsPool;
		backgroundThreadsPool.taskQueue.push_back(XTaskQueue::getGlobleQueue(XTaskPriority_High));
		backgroundThreadsPool.taskQueue.push_back(XTaskQueue::getGlobleQueue(XTaskPriority_Default));
		backgroundThreadsPool.taskQueue.push_back(XTaskQueue::getGlobleQueue(XTaskPriority_Low));
		backgroundThreadsPool.taskQueue.push_back(XTaskQueue::getGlobleQueue(XTaskPriority_Background));
		XTaskQueue::getGlobleQueue(XTaskPriority_Background)->runInPool = &backgroundThreadsPool;
		XTaskQueue::getGlobleQueue(XTaskPriority_Low)->runInPool = &backgroundThreadsPool;
		XTaskQueue::getGlobleQueue(XTaskPriority_Default)->runInPool = &backgroundThreadsPool;
		XTaskQueue::getGlobleQueue(XTaskPriority_High)->runInPool = &backgroundThreadsPool;
		for (int i = 0; i < 4; ++i) {
			backgroundThreadsPool.startNewThread();
		}
        
        source = std::make_shared<XRunLoopDispatchSource>();
        source->mTaskQueue = XTaskQueue::getMainQueue();
        getMainRunLoop()->addSource(source);
	}
	void XThreadPool::startNewThread() {
		std::thread thread(std::bind(&XThreadPool::runLoop, this));
		thread.detach();
	}
	XThreadPool::XThreadPool() {
		taskNum = 0;
	}
	XThreadPool::~XThreadPool() {
	}



	XTask::XTask(MyFun *in_fun,
		std::shared_ptr<XTaskQueue> &in_queue,
		std::chrono::time_point<std::chrono::system_clock> &&in_time) : fun(in_fun), queue(in_queue), time(in_time) {

	}
	bool XTask::operator <(const XTask& rh) {
		return time < rh.time;
	}
    XTask::~XTask() {
        
    }


	void XDispatchManager::runLoop() {
		do {
			std::unique_lock<std::mutex> lk(mutex);
			if (mQueue.empty()) {
				cv.wait(lk);
			}
			else {
				cv.wait_until(lk, (*mQueue.begin())->time);
				auto taskIter = mQueue.begin(); //应该可以不存这个，用begin取并不耗时
				auto task = *taskIter;
				auto now = std::chrono::system_clock::now();
				if (now >= task->time) {
					auto taskQueue = task->queue;
					taskQueue->push(task->fun);
					if (taskQueue->runInPool) {
						taskQueue->runInPool->onQueueChanged();
					}
					mQueue.erase(taskIter);
				}
			}
		} while (1);
	}
	MyFun* XDispatchManager::copyFunction(const MyFun &fun) {
		//拷贝function会引起很大性能损失，如何避免？  看似不能放在后台线程执行？
		MyFun*copyFun = new MyFun(fun);
		//(*copyFun)();
		//delete copyFun;
		return copyFun;
	}
	XDispatchManager* XDispatchManager::getSharedInstance() {
		static XDispatchManager *manager = new XDispatchManager();
		return manager;
	}
	XDispatchManager::XDispatchManager() {
		std::thread thread(std::bind(&XDispatchManager::runLoop, this));
		thread.detach();
	}
    void XDispatchManager::dispatchAsnyc(std::shared_ptr<XTaskQueue> taskQueue, const MyFun &fun) {
        taskQueue->push(copyFunction(fun));
        if (taskQueue->runInPool) {
            taskQueue->runInPool->onQueueChanged();
        } else {
            getMainRunLoop()->weakUp(source.get());
        }
    }
	void XDispatchManager::dispatchAfter(std::shared_ptr<XTaskQueue> taskQueue, const MyFun &fun, long delayMS) {
		XTask *task = new XTask(copyFunction(fun), taskQueue, std::chrono::system_clock::now() + std::chrono::milliseconds(delayMS));
		mutex.lock();
		//TODO::排序放到dispatch线程中执行，减少阻塞主线程时间
		mQueue.insert(std::upper_bound(mQueue.begin(), mQueue.end(), task, [](const XTask *lh, XTask *rh)->bool {
			return lh->time < rh->time;
		}), task);
		mutex.unlock();
		cv.notify_one();
	}

    XDispatchManager::~XDispatchManager() {
        
    }
}