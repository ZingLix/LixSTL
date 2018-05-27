#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

#include <gtest/gtest.h>

#include <random>
#include "../src/algorithm/heap.hpp"

template<class RandomIt,class Distance>
void test_heap(RandomIt first,Distance size) {
	for (Distance i = 3; i < size; ++i) {
		EXPECT_EQ (*(first+i) > *(first+((i - 1) / 2)),false);
	}
}

TEST(algorithm_HeapTest, make_heap) {
	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 100);

	for (int count = 0; count < 100; ++count) {
		std::vector<int> vec;

		for (int i = 0; i < 100; ++i) vec.push_back(distribution(generator));
		lix::make_heap(vec.begin(), vec.end());
		
		test_heap(vec.begin(), vec.size());
	}
}

TEST(algorithm_HeapTest, sort_heap) {
	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 1000);

	for (int count = 0; count < 100; ++count) {
		std::vector<int> vec;

		for (int i = 0; i < 100; ++i) vec.push_back(distribution(generator));
		lix::make_heap(vec.begin(), vec.end());

		lix::sort_heap(vec.begin(), vec.end());
		bool flag = true;
		auto tmp = vec[0];
		for(auto it=vec.begin();it!=vec.end();++it) {
			if (tmp > *it) { flag = false; break; }
			tmp = *it;
		}
		EXPECT_EQ(flag, true);
	}
}

TEST(algorithm_HeapTest, is_heap) {
	std::vector<int> vec{ 43,73,24,63,78,22,64 };
	EXPECT_EQ(lix::is_heap(vec.begin(),vec.end()), false);
	lix::make_heap(vec.begin(), vec.end());
	EXPECT_EQ(lix::is_heap(vec.begin(),vec.end()), true);
	vec[0] = -3;
	EXPECT_EQ(lix::is_heap(vec.begin(), vec.end()), false);
	lix::make_heap(vec.begin(), vec.end());
	EXPECT_EQ(lix::is_heap(vec.begin(), vec.end()), true);
	vec[6] = 999;
	EXPECT_EQ(lix::is_heap(vec.begin(), vec.end()), false);
}