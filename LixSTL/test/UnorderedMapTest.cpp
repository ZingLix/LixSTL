#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <gtest/gtest.h>
#include "../src/container/unordered_map.hpp"
#include <random>

TEST(UnorderedMapTest, insert) {
	std::vector<int> key{ 9,58,15,36,48,14 };
	std::vector<std::string> value{ "a","b","c","d","e","f" };
	
	lix::unordered_map<int,std::string> map;

	auto it1 = key.begin();
	auto it2 = value.begin();
	for (; it1 != key.end(); ++it1,++it2) {
		auto ret = map.insert(std::make_pair(*it1,*it2));
		EXPECT_EQ(*ret.first, std::make_pair(*it1,*it2));
		EXPECT_EQ(ret.second, true);
	}

	it1 = key.begin();
	it2 = value.begin();

	for (; it1 != key.end(); ++it1, ++it2) {
		auto ret = map.insert(std::make_pair(*it1, *it2));
		EXPECT_EQ(*ret.first, std::make_pair(*it1, *it2));
		EXPECT_EQ(ret.second, false);
	}

	it1 = key.begin();
	it2 = value.begin();

	for (; it1 != key.end(); ++it1, ++it2) {
		auto ret = map.find(*it1);
		EXPECT_EQ(*ret, std::make_pair(*it1, *it2));
	}

	std::vector<int> errarr{ 5,78,18 };
	for (auto it = errarr.begin(); it != errarr.end(); ++it) {
		auto ret = map.find(*it);
		EXPECT_EQ(ret, map.end());
	}
}