#include <iostream>
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <memory>
#include <gtest/gtest.h>
#include <random>
#include <deque>
#include <queue>

#include "../src/container/map.hpp"

int main(int ac, char* av[])
{
	testing::InitGoogleTest(&ac, av);
	::testing::GTEST_FLAG(filter) = "MapTest*";
	//while (true) 
	RUN_ALL_TESTS();
	//std::make_pair()
	//lix::map<int, std::string> map;
	//std::string s = "213";
	/*auto p = std::make_pair(1, "123123");
	map.insert( p );
	std::cout << (*(map.begin())).first;
	map.erase(p.first);*/
	system("pause");
	return 0;
}
