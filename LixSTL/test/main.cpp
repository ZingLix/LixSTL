#include "../include/vector"

#include <iostream>


#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <memory>
#include <gtest/gtest.h>

template<class T>
void print(lix::vector<T>& vec) {
	for(auto itr=vec.begin();itr!=vec.end();++itr) {
		std::cout << *itr;
	}
	std::cout << std::endl;

}


int main(int ac, char* av[])
{
	
	testing::InitGoogleTest(&ac, av);
	RUN_ALL_TESTS();

	system("pause");
	return 0;
}
