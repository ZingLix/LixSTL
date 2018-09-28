#include <iostream>
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <memory>
#include <gtest/gtest.h>
#include <random>
#include <deque>
#include <queue>

#include "../src/algorithm/heap.hpp"
#include "../src/container/vector.hpp"

void print(lix::vector<int>& vec) {
	for(auto it=vec.begin();it!=vec.end();++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

int main(int ac, char* av[])
{
	testing::InitGoogleTest(&ac, av);
	::testing::GTEST_FLAG(filter) = "HashTableTest*";
	while (true) 
	RUN_ALL_TESTS();

	system("pause");
	return 0;
}

