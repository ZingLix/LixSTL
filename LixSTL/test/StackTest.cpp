#include "../include/stack"
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#include <gtest/gtest.h>

TEST(StackTest,test) {
	lix::stack<int> stack;
	stack.push(5);
	stack.push(9);
	stack.push(3);
	EXPECT_EQ(stack.empty(), false);
	EXPECT_EQ(stack.size(),3);
	EXPECT_EQ(stack.top(), 3);
	stack.pop();
	EXPECT_EQ(stack.size(), 2);
	EXPECT_EQ(stack.top(), 9);
	stack.pop();
	EXPECT_EQ(stack.size(), 1);
	EXPECT_EQ(stack.top(), 5);
	stack.pop();
	EXPECT_EQ(stack.size(), 0);
	EXPECT_EQ(stack.empty(), true);
}