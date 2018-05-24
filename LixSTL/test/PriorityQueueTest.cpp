#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

#include <gtest/gtest.h>

#include <random>
#include "../include/queue"
#include <queue>

TEST(PriorityQueueTest, test) {
	lix::priority_queue<int> lixqueue;
	std::priority_queue<int> stdqueue;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	std::vector<int> ex{ 65,25,96,24,35 };

	for (int i = 0; i<5000; i++) {
		dice_roll = distribution(generator);
		lixqueue.push(dice_roll);
		stdqueue.push(dice_roll);
	}


	while (!lixqueue.empty()) {
		EXPECT_EQ(lixqueue.top(), stdqueue.top());
	//	std::cout << lixqueue.top()<<" ";
		lixqueue.pop();
		stdqueue.pop();
	}
	EXPECT_EQ(stdqueue.empty(), lixqueue.empty());
}