#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <gtest/gtest.h>
#include "../src/container/queue.hpp"

TEST(QueueTest, test) {
	lix::queue<int> queue;
	queue.push(5);
	queue.push(9);
	queue.push(3);
	EXPECT_EQ(queue.empty(), false);
	EXPECT_EQ(queue.size(), 3);
	EXPECT_EQ(queue.front(), 5);
	EXPECT_EQ(queue.back(), 3);
	queue.pop();
	EXPECT_EQ(queue.size(), 2);
	EXPECT_EQ(queue.front(), 9);
	EXPECT_EQ(queue.back(), 3);
	queue.pop();
	EXPECT_EQ(queue.size(), 1);
	EXPECT_EQ(queue.front(), 3);
	EXPECT_EQ(queue.back(), 3);
	queue.pop();
	EXPECT_EQ(queue.size(), 0);
	EXPECT_EQ(queue.empty(), true);
}