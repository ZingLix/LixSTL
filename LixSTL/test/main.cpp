#include <iostream>
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <memory>
#include <gtest/gtest.h>
#include <random>
#include <deque>
#include <queue>

#include "../src/container/priority_queue.hpp"

int main(int ac, char* av[])
{
	testing::InitGoogleTest(&ac, av);
	::testing::GTEST_FLAG(filter) = "algorithm_HeapTest*";
//	while (true) 
	RUN_ALL_TESTS();

	//std::vector<int> ex{ 65,25,96,24,35,87,23 };
	//lix::priority_queue<int> lixqueue;
	//for (int i = 0; i<ex.size(); i++) {
	//	lixqueue.push(ex[i]);
	//	//lixqueue.print();
	//	//std::cout << std::endl;
	//}
	//lix::make_heap(ex.begin(), ex.end());
	//lix::sort_heap(ex.begin(), ex.end());
	//for (int i = 0; i<ex.size(); i++) {
	//	std::cout << ex[i]<<" ";
	//}
	system("pause");
	return 0;
}
