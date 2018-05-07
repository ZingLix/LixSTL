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

int main(int ac, char* av[])
{
	testing::InitGoogleTest(&ac, av);
	::testing::GTEST_FLAG(filter) = "DequeTest*";
	//while (true) 
	RUN_ALL_TESTS();
	//lix::vector<lix::list<int>> asdf;
//	lix::deque<int> test(999999, 45);
//	system("pause");
//	for (int i = 0; i < 999990; ++i) test.pop_front();
//	test.size();
//	system("pause");
////	test.shrink_to_fit();
//	system("pause");
	return 0;
}
