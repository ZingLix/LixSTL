#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

#include <gtest/gtest.h>

#include <random>
#include "../src/container/list.hpp"
#include <list>
#include "../src/container/vector.hpp"

template<class T>
void listEqualTest(lix::list<T>& lixlist,std::list<T>& stdlist) {
	auto itr1 = lixlist.begin();
	auto itr2 = stdlist.begin();
	for(;itr2!=stdlist.end();++itr1,++itr2) {
		EXPECT_EQ(*itr1, *itr2);
	}
}

TEST(ListTest,Init) {
	lix::list<int> lixlist;
	std::list<int> stdlist;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for(int i=0;i<50000;i++) {
		dice_roll = distribution(generator);
		lixlist.push_back(dice_roll);
		stdlist.push_back(dice_roll);
	}

	listEqualTest(lixlist, stdlist);
}

TEST(ListTest, Func_back_and_front) {
	lix::list<int> lixlist;
	std::list<int> stdlist;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for (int i = 0; i<5000; i++) {
		dice_roll = distribution(generator);
		lixlist.push_back(dice_roll);
		stdlist.push_back(dice_roll);
	}

	EXPECT_EQ(lixlist.back(), stdlist.back());
	EXPECT_EQ(lixlist.front(), stdlist.front());
}

TEST(ListTest, Func_clear) {
	lix::list<int> lixlist;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for (int i = 0; i<5000; i++) {
		dice_roll = distribution(generator);
		lixlist.push_back(dice_roll);
	}
	lixlist.clear();

	EXPECT_EQ(lixlist.empty(), true);
	EXPECT_EQ(lixlist.begin(), lixlist.end());
}

TEST(ListTest, Func_erase) {
	lix::list<int> lixlist;
	std::list<int> stdlist;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for (int i = 0; i<5000; i++) {
		dice_roll = distribution(generator);
		lixlist.push_back(dice_roll);
		stdlist.push_back(dice_roll);
	}
	
	std::uniform_int_distribution<int> distribution1(0, static_cast<int>(lixlist.size()));
	dice_roll = distribution1(generator);

	auto lixlistitr = lixlist.begin();
	auto stdlistitr = stdlist.begin();

	for(int i=0;i<dice_roll;i++) {
		++lixlistitr;
		++stdlistitr;
	}
	lixlist.erase(lixlistitr);
	stdlist.erase(stdlistitr);

	listEqualTest(lixlist, stdlist);

	//test for erase(itr1,itr2)
	/*std::uniform_int_distribution<int> distribution2(0, static_cast<int>(lixlist.size()));
	dice_roll = distribution2(generator);
	int dice_roll2 = distribution2(generator);

	if(dice_roll>dice_roll2) {
		auto tmp = dice_roll;
		dice_roll = dice_roll2;
		dice_roll2 = tmp;
	}

	lixlistitr = lixlist.begin();
	stdlistitr = stdlist.begin();

	int i = 0;
	for (i = 0; i<dice_roll; i++) {
		++lixlistitr;
		++stdlistitr;
	}
	auto lixlistitr2 = lixlistitr;
	auto stdlistitr2 = stdlistitr;
	for (; i<dice_roll2; i++) {
		++lixlistitr;
		++stdlistitr;
	}
	lixlist.erase(lixlistitr2,lixlistitr);
	stdlist.erase(stdlistitr2,stdlistitr);

	listEqualTest(lixlist, stdlist);*/
}

TEST(ListTest,Func_insert) {
	lix::list<int> lixlist;
	std::list<int> stdlist;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for (int i = 0; i<5000; i++) {
		dice_roll = distribution(generator);
		lixlist.push_front(dice_roll);
		stdlist.push_front(dice_roll);
	}
	dice_roll = distribution(generator);
	dice_roll /= 2;
	auto itr1 = lixlist.begin();
	auto itr2 = stdlist.begin();
	while(dice_roll--) {
		++itr1;
		++itr2;
	}
	lixlist.insert(itr1, 5888);
	stdlist.insert(itr2, 5888);

	listEqualTest(lixlist, stdlist);
}

TEST(ListTest,Func_pop) {
	lix::list<int> lixlist;
	std::list<int> stdlist;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for (int i = 0; i<5000; i++) {
		dice_roll = distribution(generator);
		lixlist.push_front(dice_roll);
		stdlist.push_front(dice_roll);
	}

	std::uniform_int_distribution<int> distributio1(0, static_cast<int>(lixlist.size()));
	dice_roll = distributio1(generator);
	while(dice_roll--) {
		lixlist.pop_back();
		stdlist.pop_back();
	}
	listEqualTest(lixlist, stdlist);

	std::uniform_int_distribution<int> distributio2(0, static_cast<int>(lixlist.size()));
	dice_roll = distributio2(generator);
	while (dice_roll--) {
		lixlist.pop_front();
		stdlist.pop_front();
	}
	listEqualTest(lixlist, stdlist);
}

TEST(ListTest,Func_remove) {
	lix::list<int> lixlist;
	std::list<int> stdlist;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	lix::vector<int> idx;

	for (int i = 0; i<5000; i++) {
		dice_roll = distribution(generator);
		lixlist.push_front(dice_roll);
		stdlist.push_front(dice_roll);
		if (i % 500 == 3) idx.push_back(dice_roll);
	}

	for (auto itr = idx.begin(); itr != idx.end();++itr) {
		lixlist.remove(*itr);
		stdlist.remove(*itr);
		listEqualTest(lixlist, stdlist);
	}
}