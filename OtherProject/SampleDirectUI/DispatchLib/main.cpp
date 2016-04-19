#include "XDispatch.h"
using namespace XDispatch;
void 性能测试() {
	srand(5246);
	clock_t t1 = clock();
	std::atomic_int_fast64_t totalNumber = 0, number1 = 0, number2 = 0;
	int i = 0;
	auto queue1 = XTaskQueue::getGlobleQueue(XTaskPriority_Default);
	XDispatchManager* manager = XDispatchManager::getSharedInstance();
	for (; i < 1000000; ++i)
	{
		int ms = rand() % 2000;
		if (ms > 1000)
		{
			manager->dispatchAsnyc(queue1, [&totalNumber, &number1]{
				//for (int i = 0; i < 1000; ++i)
				{
					++totalNumber;
					++number1;
				}
			});
		}
		else
		{
			XDispatchManager::getSharedInstance()->dispatchAfter(XTaskQueue::getGlobleQueue(XTaskPriority_High), [&totalNumber, &number2]{
				{
					++totalNumber;
					++number2;
				}
			}, ms);
		}
	}
	clock_t t2 = clock();
	std::cout << "count:" << i << "   time:" << (t2 - t1) / (CLOCKS_PER_SEC / 1000.0) << "ms" << std::endl;
	std::cout << "total:" << totalNumber << "   number1:" << number1 << "   number2:" << number2 << std::endl;
}

void main() {
	XThreadPool::initGlobelPool();
	XDispatchManager::getSharedInstance()->dispatchAfter(XTaskQueue::getMainQueue(), []{
		std::cout << "test1";
		XDispatchManager::getSharedInstance()->dispatchAfter(XTaskQueue::getMainQueue(), []{
			std::cout << "test2";
			XDispatchManager::getSharedInstance()->dispatchAfter(XTaskQueue::getMainQueue(), []{
				std::cout << "test3";

			}, 3000);
		}, 1000);
	}, 600);
	
	int a = 0;
	std::cin >> a;
}

/*
Release:(5740 拷贝fun所用时间)
//空代码163ms  1千万次循环
1744
22000

Debug:(2115 拷贝fun所用时间)
//空代码1087ms 1百万次
std队列push速度  1690ms

*/