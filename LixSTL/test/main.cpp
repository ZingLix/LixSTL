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

int main(int ac, char* av[])
{
	testing::InitGoogleTest(&ac, av);
	::testing::GTEST_FLAG(filter) = "RbTreeTest*";
//	while (true) 
	RUN_ALL_TESTS();

	//lix::rb_tree<int, int, lix::identity<int>, std::less<int>> tree;
	//EXPECT_EQ(tree.size(), 0);
//	for (int i = 10; i > 0; --i) {
	//	tree.insert(i);
	//	tree.print();
	//	std::cout << tree.test() << std::endl;
	//}
	//for (int i = 10; i > 0; --i) {
	//	tree.remove(i);
	//	tree.print();
	//	std::cout <<tree.test()<< std::endl;
	//}
	system("pause");
	return 0;
}
