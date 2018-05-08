#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

#include <gtest/gtest.h>

#include <random>

#include "../src/container/deque.hpp"
#include <deque>

template<class T>
bool dequeEqualTest1(lix::deque<T>& lixdeque, std::deque<T>& stddeque) {
	auto itr1 = lixdeque.begin();
	auto itr2 = stddeque.begin();
	for (; itr2 != stddeque.end(); ++itr1, ++itr2) {
		EXPECT_EQ(*itr1, *itr2);
	}
	return  itr1 == lixdeque.end();
}

template<class T>
void dequeEqualTest(lix::deque<T>& lixdeque, std::deque<T>& stddeque) {
	EXPECT_EQ(dequeEqualTest1(lixdeque, stddeque), true);
}

TEST(DequeTest,Init) {
	lix::deque<int> lixdeque;
	std::deque<int> stddeque;
	EXPECT_EQ(lixdeque.size(), stddeque.size());
	EXPECT_EQ(lixdeque.empty(), stddeque.empty());

	lix::deque<int> lixdeque2(99999, 88);
	std::deque<int> stddeque2(99999, 88);
	dequeEqualTest(lixdeque2, stddeque2);

	lix::deque<int> lixdeque3(lixdeque2);
	dequeEqualTest(lixdeque3, stddeque2);

	lix::deque<int> lixdeque4(std::move(lixdeque2));
	dequeEqualTest(lixdeque4, stddeque2);

	lix::deque<int> lixdeque5{ 5,3,76 };
	std::deque<int> stddeque5{ 5,3,76 };
	dequeEqualTest(lixdeque5, stddeque5);
}

TEST(DequeTest,Func_push) {
	lix::deque<int> lixdeque(3,10);
	std::deque<int> stddeque(3,10);

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for (int i = 0; i<5000; i++) {
		dice_roll = distribution(generator);
		lixdeque.push_front(dice_roll);
		stddeque.push_front(dice_roll);
	}

	dequeEqualTest(lixdeque, stddeque);
}

TEST(DequeTest, Func_pop) {
	lix::deque<int> lixdeque;
	std::deque<int> stddeque;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for (int i = 0; i<5000; i++) {
		dice_roll = distribution(generator);
		lixdeque.push_front(dice_roll);
		stddeque.push_front(dice_roll);
	}

	for(int i=0;i<5000;++i) {
		lixdeque.pop_back();
		stddeque.pop_back();
		if (i % 500 == 54) dequeEqualTest(lixdeque, stddeque);
	}

	EXPECT_EQ(lixdeque.empty(), true);
	EXPECT_EQ(stddeque.empty(), true);

	for (int i = 0; i<5000; i++) {
		dice_roll = distribution(generator);
		lixdeque.push_front(dice_roll);
		stddeque.push_front(dice_roll);
	}

	//for (int i = 0; i<5000; ++i) {
	//	lixdeque.pop_front();
	//	stddeque.pop_front();
	//	if (i % 50 == 4) dequeEqualTest(lixdeque, stddeque);
	//}
	//EXPECT_EQ(lixdeque.empty(), true);
}

TEST(DequeTest, Func_front_and_back) {
	lix::deque<int> lixdeque;
	std::deque<int> stddeque;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for (int i = 0; i<50; i++) {
		dice_roll = distribution(generator);
		lixdeque.push_front(dice_roll);
		stddeque.push_front(dice_roll);
	}

	EXPECT_EQ(lixdeque.front(), stddeque.front());
	EXPECT_EQ(lixdeque.back(), stddeque.back());
}

TEST(DequeTest,Func_empty) {
	lix::deque<int> lixdeque;
	std::deque<int> stddeque;
	EXPECT_EQ(lixdeque.empty(), stddeque.empty());

	lixdeque.push_back(3);
	stddeque.push_back(3);
	EXPECT_EQ(lixdeque.empty(), stddeque.empty());

	lixdeque.clear();
	stddeque.clear();
	EXPECT_EQ(lixdeque.empty(), stddeque.empty());
}

TEST(DequeTest,Operator_equal) {
	lix::deque<int> lixdeque{ 6,4,32,672,45345 };
	std::deque<int> stddeque{ 6,4,32,672,45345 };
	lix::deque<int> lixdeque1{ 124,1346,1345 };
	lixdeque1 = lixdeque;
	dequeEqualTest(lixdeque1, stddeque);

	lixdeque1 = { 534,6234,2345 };
	std::deque<int> stddeque1{ 534,6234,2345 };
	dequeEqualTest(lixdeque1, stddeque1);

	lixdeque1 = std::move(lixdeque);
	dequeEqualTest(lixdeque1, stddeque);
}

TEST(DequeTest, Operator_square) {
	lix::deque<int> lixdeque(3, 10);
	std::deque<int> stddeque(3, 10);

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for (int i = 0; i<5000; i++) {
		dice_roll = distribution(generator);
		lixdeque.push_front(dice_roll);
		stddeque.push_front(dice_roll);
	}

	for(int i=0;i<lixdeque.size();++i) {
		EXPECT_EQ(lixdeque[i], stddeque[i]);
		EXPECT_EQ(lixdeque.at(i), stddeque.at(i));
	}
}

TEST(DequeTest,Func_insert) {
	lix::deque<int> lixdeque(3, 10);
	std::deque<int> stddeque(3, 10);

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for (int i = 0; i<5000; i++) {
		dice_roll = distribution(generator);
		lixdeque.push_front(dice_roll);
		stddeque.push_front(dice_roll);
	}

	std::uniform_int_distribution<int> distribution1(0, 5000);
	int idx;
	for(int i=0;i<100;++i) {
		idx = distribution1(generator);
		dice_roll = distribution(generator);
		lixdeque.insert(lixdeque.begin()+idx, dice_roll);
		stddeque.insert(stddeque.begin() + idx, dice_roll);
	}
	dequeEqualTest(lixdeque, stddeque);
}