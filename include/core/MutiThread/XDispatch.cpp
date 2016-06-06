#include "XDispatch.h"
#include "XRunLoop.hpp"

namespace XDispatch {
    
    std::shared_ptr<XRunLoopDispatchSource> XDispatchManager::mMainThreadSource = nullptr;

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
					} else {
						getMainRunLoop()->weakUp(mMainThreadSource.get());
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
            getMainRunLoop()->weakUp(mMainThreadSource.get());
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