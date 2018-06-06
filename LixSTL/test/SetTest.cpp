#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

#include <gtest/gtest.h>

#include "../src/container/set.hpp"
#include <random>

template<class T>
void setTest(lix::set<T>& lixset, std::set<T>& stdset) {
	auto lixit = lixset.begin();
	auto stdit = stdset.begin();
	for(;stdit!=stdset.end();++lixit,++stdit) {
		EXPECT_EQ(*lixit, *stdit);
	}
	EXPECT_EQ(lixset.size(), stdset.size());
	EXPECT_EQ(lixit == lixset.end(), true);
}

lix::set<int>::iterator add(lix::set<int>::iterator it, int i) {
	while (i--) ++it;
	return it;
}

std::set<int>::iterator add(std::set<int>::iterator it, int i) {
	while (i--) ++it;
	return it;
}

TEST(SetTest, init) {
	lix::set<int> lixset{ 8,4,75,32,98,24,12,35 };
	std::set<int> stdset{ 8,4,75,32,98,24,12,35 };
	setTest(lixset, stdset);

	lix::set<int> lixset1(lixset.begin(), lixset.end());
	setTest(lixset1, stdset);

	lix::set<int> lixset2(lixset);
	setTest(lixset2, stdset);
}

TEST(SetTest, operator_equal) {
	lix::set<int> lixset{ 8,4,75,32,98,24,12,35 };
	std::set<int> stdset{ 8,4,75,32,98,24,12,35 };

	lix::set<int> lixset1=lixset;
	setTest(lixset1, stdset);

	std::set<int> stdset1{ 95,47,25,6,4,8,63 };
	lix::set<int> lixset2{ 123 };
	lixset2 = { 95,47,25,6,4,8,63 };
	setTest(lixset2, stdset1);
}

TEST(SetTest, insert_and_erase) {
	lix::set<int> lixset;
	std::set<int> stdset;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for(int i=0;i<1000;++i) {
		dice_roll = distribution(generator);
		lixset.insert(dice_roll);
		stdset.insert(dice_roll);
	}
	setTest(lixset, stdset);

	std::uniform_int_distribution<int> distribution1(1000, 1100);
	for (int i = 0; i<1000; ++i) {
		dice_roll = distribution1(generator);
		lixset.insert(dice_roll);
		stdset.insert(dice_roll);
	}
	setTest(lixset, stdset);

	for(int i=0;i<10005;++i) {
		EXPECT_EQ(lixset.erase(i),stdset.erase(i));
	}
	setTest(lixset, stdset);
	EXPECT_EQ(lixset.empty(), true);

	for (int i = 0; i<1000; ++i) {
		dice_roll = distribution(generator);
		lixset.insert(dice_roll);
		stdset.insert(dice_roll);
	}
	setTest(lixset, stdset);
}

TEST(SetTest, insert_erase_related) {
	lix::set<int> lixset{ 56,48,35,28,95,65,45,25 };
	std::set<int> stdset{ 56,48,35,28,95,65,45,25 };

	lix::set<int> lixset1{ 15,49,35,68,99,15,48 };
	std::set<int> stdset1{ 15,49,35,68,99,15,48 };

	lixset.insert({ 26,48,58,47,65,29,98 });
	stdset.insert({ 26,48,58,47,65,29,98 });
	setTest(lixset, stdset);

	lixset.insert(lixset1.begin(), lixset1.end());
	stdset.insert(stdset1.begin(), stdset1.end());
	setTest(lixset, stdset);

	lix::set<int>::iterator lixit;
	std::set<int>::iterator stdit;

	for (int i = 0; i < 4; ++i) {
		lixit = lixset.erase(add(lixset.begin(), 2));
		stdit = stdset.erase(add(stdset.begin(), 2));
		setTest(lixset, stdset);
		EXPECT_EQ(*lixit, *stdit);
	}

	lixit= lixset.erase(add(lixset.begin(), 3), add(lixset.begin(), 8));
	stdit= stdset.erase(add(stdset.begin(), 3), add(stdset.begin(), 8));
	setTest(lixset, stdset);
	EXPECT_EQ(*lixit, *stdit);
}