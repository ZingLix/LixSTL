#include "../include/vector"
//#include <vector>
#include <iostream>
//#include <list>
//#include "../src/list.hpp"
//#include "../src/deque.hpp"

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <gtest/gtest.h>
//#include "VectorTest.cpp"
#include <memory>

template<class T>
void print(lix::vector<T>& vec) {
	for(auto itr=vec.begin();itr!=vec.end();++itr) {
		std::cout << *itr;
	}
	std::cout << std::endl;

}

int main(int ac, char* av[])
{
//	std::pointer_traits<int> it;
	testing::InitGoogleTest(&ac, av);
	RUN_ALL_TESTS();

	system("pause");
	//return 0;
	//lix::vector<int> vec;
	//vec.push_back(123);
	//vec.push_back("asdf");
	//vec.push_back("1234");
	//print(vec);
	//system("pause");
	return 0;
}
