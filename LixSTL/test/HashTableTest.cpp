#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <gtest/gtest.h>

#include "../src/container/hash_table.hpp"
#include "utility"
#include "../src/algorithm/algorithm.hpp"


TEST(HashTableTest,Init) {
	lix::hash_table<int, int, std::hash<int>, lix::identity<int>, std::equal_to<int>>
	ht(50, std::hash<int>(), std::equal_to<int>());

	EXPECT_EQ(ht.size(),0);
	EXPECT_EQ(ht.bucket_count(), 53);

	std::vector<int> arr{ 59,63,108,2,53,55 };

	for(auto it = arr.begin();it!=arr.end();++it) {
		ht.insert_unique(*it);
	}
	EXPECT_EQ(ht.size(), 6);

	ht.find(59) == ht.end();
}