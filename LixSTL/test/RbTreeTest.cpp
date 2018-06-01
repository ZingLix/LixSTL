#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <gtest/gtest.h>
#include "../src/container/rb_tree.hpp"

TEST(RbTreeTest, insert) {
	lix::rb_tree<int, int, lix::identity<int>, std::less<int>> tree;
	EXPECT_EQ(tree.size(), 0);
	for (int i = 0; i < 10; ++i) tree.insert(i);
}