#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <gtest/gtest.h>
#include "../src/container/rb_tree.hpp"
#include <random>
#include "../src/container/vector.hpp"

TEST(RbTreeTest, insert) {
	lix::rb_tree<int, std::less<int>> tree;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);
	std::vector<int> vec;

	EXPECT_EQ(tree.size(), 0);
	for (int i = 0; i < 1000; ++i) {
		dice_roll = distribution(generator);
		vec.push_back(dice_roll);
		tree.insert(dice_roll);
		EXPECT_EQ(tree.test(), true);
	}
	
	for(int i=1000;i>0;--i) {
		std::uniform_int_distribution<int> dis(0, i-1);
		dice_roll = dis(generator);
		tree.remove(vec[dice_roll]);
		vec.erase(vec.begin()+ dice_roll);
		EXPECT_EQ(tree.test(), true);
		EXPECT_EQ(tree.size(), i - 1);
	}
}