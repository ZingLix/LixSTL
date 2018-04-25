#include "../include/vector"

#include <iostream>

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <memory>
#include <gtest/gtest.h>
#include "../src/container/list.hpp"
#include "../src/container/deque.hpp"
#include <random>


int main(int ac, char* av[])
{
	
	testing::InitGoogleTest(&ac, av);
	::testing::GTEST_FLAG(filter) = "DequeTest*Func_pop";
	//while(true)
	//RUN_ALL_TESTS();
	//lix::list<int> list;
	//list.push_back(123);
	//list.clear();
	//lix::deque<int> lixdeque;

	//std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	//std::uniform_int_distribution<int> distribution(0, 100);
	//int dice_roll = distribution(generator);

	//for (int i = 0; i<10; i++) {
	//	dice_roll = distribution(generator);
	//	lixdeque.push_front(dice_roll);
	//}

	//for(auto it=lixdeque.begin();it!=lixdeque.end();++it) {
	//	std::cout << *it << " ";
	//}

	system("pause");
	lix::deque<int>* lixd = new lix::deque<int>(10000000, 0);

	system("pause");
	delete lixd;
	system("pause");
	return 0;
}
