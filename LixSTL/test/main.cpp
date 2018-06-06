#include <iostream>
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <memory>
#include <gtest/gtest.h>
#include <random>
#include <deque>
#include <queue>

#include "../src/container/priority_queue.hpp"
#include "../src/container/rb_tree.hpp"
#include "../src/container/set.hpp"

int main(int ac, char* av[])
{
	testing::InitGoogleTest(&ac, av);
//	::testing::GTEST_FLAG(filter) = "SetTest*";
	while (true) 
	RUN_ALL_TESTS();

//	lix::set<int> set;
//	set.insert(3);
//	set.erase(3);
//	set.erase(4);
	
	system("pause");
	return 0;
}
