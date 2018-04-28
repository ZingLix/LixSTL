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
	lix::vector<int> dd;
	dd.swap(dd);
	testing::InitGoogleTest(&ac, av);
	::testing::GTEST_FLAG(filter) = "VectorTest*";
	//while (true) 
	RUN_ALL_TESTS();
	system("pause");
	return 0;
}
