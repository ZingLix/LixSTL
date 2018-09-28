#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <gtest/gtest.h>
#include "../src/container/unordered_set.hpp"
#include <random>

TEST(UnorderedSetTest, insert) {
	std::vector<int> arr{ 9,58,15,36,48,14 };
	lix::unordered_set<int> set;
	for(auto it=arr.begin();it!=arr.end();++it) {
		auto ret = set.insert(*it);
		EXPECT_EQ(*ret.first, *it);
		EXPECT_EQ(ret.second, true);
	}

	for (auto it = arr.begin(); it != arr.end(); ++it) {
		auto ret = set.insert(*it);
		EXPECT_EQ(*ret.first, *it);
		EXPECT_EQ(ret.second, false);
	}

	for (auto it = arr.begin(); it != arr.end(); ++it) {
		auto ret = set.find(*it);
		EXPECT_EQ(*ret, *it);
	}
	std::vector<int> errarr{ 5,78,18 };
	for (auto it = errarr.begin(); it != errarr.end(); ++it) {
		lix::unordered_set<int>::iterator ret = set.find(*it);
		EXPECT_EQ(ret , set.end());
	}
}