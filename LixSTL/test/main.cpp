#include "../include/vector"

#include <iostream>

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <memory>
#include <gtest/gtest.h>
#include "../src/container/list.hpp"
#include "../src/container/deque.hpp"
#include <random>
#include <deque>
#include <queue>
#include "../src/container/priority_queue.hpp"

int main(int ac, char* av[])
{
	testing::InitGoogleTest(&ac, av);
	::testing::GTEST_FLAG(filter) = "PriorityQueueTest*";
	while (true) 
	RUN_ALL_TESTS();

	system("pause");
	return 0;
}
