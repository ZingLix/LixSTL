#include "../src/vector.hpp"
//#include <vector>
#include <iostream>
//#include <list>
#include "../src/list.hpp"
#include "../src/deque.hpp"

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <gtest/gtest.h>
//#include "VectorTest.cpp"


int main(int ac, char* av[])
{
	testing::InitGoogleTest(&ac, av);
	RUN_ALL_TESTS();
	system("pause");
	return 0;
}