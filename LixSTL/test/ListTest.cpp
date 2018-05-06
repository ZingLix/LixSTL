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

	for(int i=0;i<5000;i++) {
		dice_roll = distribution(generator);
		lixlist.push_back(dice_roll);
		stdlist.push_back(dice_roll);
	}

	listEqualTest(lixlist, stdlist);

	lix::list<int> lixlist2(5, 66);
	std::list<int> stdlist2(5, 66);
	listEqualTest(lixlist2, stdlist2);

	lix::list<std::string> lixlist3(5);
	std::list<std::string> stdlist3(5);
	listEqualTest(lixlist3, stdlist3);

	/*lix::list<std::string> lixlist4(lixlist2.begin(), lixlist2.end());
	std::list<std::string> stdlist4(stdlist2.begin(), stdlist2.end());
	listEqualTest(lixlist4, stdlist4);*/

	lix::list<int> lixlist5(lixlist2);
	std::list<int> stdlist5(stdlist2);
	listEqualTest(lixlist5, stdlist5);

	lix::list<int> lixlist6 = std::move(lixlist2);
	std::list<int> stdlist6 = std::move(stdlist2);
	listEqualTest(lixlist6, stdlist6);

	lix::list<std::string> lixlist7{ "a","bb","ccc" };
	std::list<std::string> stdlist7{ "a","bb","ccc" };
	listEqualTest(lixlist7, stdlist7);
}

TEST(ListTest,Operator_equal) {
	lix::list<int> list(5000, 500);
	lix::list<int> tmp1{ 1,2,3 };
	std::list<int> ex1{ 1,2,3 };
	list = tmp1;
	listEqualTest(list, ex1);

	lix::list<int> tmp2{ 4,5,6 };
	std::list<int> ex2{ 4,5,6 };
	list = std::move(tmp2);
	listEqualTest(list, ex2);

	list = { 1,2,3 };
	listEqualTest(list, ex1);
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
	std::uniform_int_distribution<int> distribution2(0, static_cast<int>(lixlist.size()));
	dice_roll = distribution2(generator);
	int dice_roll2 = distribution2(generator);

	if(dice_roll>dice_roll2) {
		auto tmp = dice_roll;
		dice_roll = dice_roll2;
		dice_roll2 = tmp;
	}

	lixlistitr = lixlist.begin();
	stdlistitr = stdlist.begin();

	int i;
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

//	listEqualTest(lixlist, stdlist);
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

	lix::list<int> lixlist2{ 1,2,3,4,5 };
	std::list<int> stdlist2{ 1,2,3,4,5 };
	int x = 2;
	auto itl = lixlist2.begin();
	auto its = stdlist2.begin();
	while (x--) {
		++itl; ++its;
	}
	auto test= lixlist2.insert(itl, 5, 99);
	stdlist2.insert(its, 5, 99);
	listEqualTest(lixlist2, stdlist2);
	EXPECT_EQ(*(--test), 2);

	++itl; ++its;
	test = lixlist2.insert(itl, { 9,8,7 });
	stdlist2.insert(its, { 9,8,7 });
	listEqualTest(lixlist2, stdlist2);
	EXPECT_EQ(*(--test), 3);
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

TEST(ListTest,Func_emplace) {
	struct test
	{
		test(int t) { x = t + 1; }
		int x;
	};

	test a(2);
	lix::list<test> list(3, a);
	auto it = list.begin();
	++it;
	list.emplace(it, 99);
	EXPECT_EQ((*it).x, 3);
	--it;
	EXPECT_EQ((*it).x, 100);

	list.emplace_back(66);
	it = list.end();
	EXPECT_EQ((*(--it)).x, 67);

	list.emplace_front(44);
	it = list.begin();
	EXPECT_EQ((*(it)).x, 45);
}

TEST(ListTest, Func_swap) {
	lix::list<int> lixlist1{ 1,2 };
	lix::list<int> lixlist2{ 4,5 };
	lixlist1.swap(lixlist2);

	std::list<int> stdlist1{ 1,2 };
	std::list<int> stdlist2{ 4,5 };

	listEqualTest(lixlist1, stdlist2);
	listEqualTest(lixlist2, stdlist1);
}

TEST(ListTest,Func_merge) {
	lix::list<int> lixlist1{ 1,1,6,9 };
	lix::list<int> lixlist2{ 0,3,4,6,8};
	//lixlist1.sort();
	//lixlist2.sort();
	lixlist1.merge(lixlist2);

	std::list<int> stdlist1{ 1,1,6,9 };
	std::list<int> stdlist2{ 0,3,4,6,8 };
	//stdlist1.sort();
	//stdlist2.sort();
	stdlist1.merge(stdlist2);

	listEqualTest(lixlist1, stdlist1);
	EXPECT_EQ(lixlist2.empty(), stdlist2.empty());

	lixlist1.merge(lixlist1);
	listEqualTest(lixlist1, stdlist1);
}

TEST(ListTest,Func_reverse) {
	lix::list<int> lixlist;
	std::list<int> stdlist;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for (int i = 0; i<500; i++) {
		dice_roll = distribution(generator);
		lixlist.push_back(dice_roll);
		stdlist.push_back(dice_roll);
	}
	lixlist.reverse();
	stdlist.reverse();
	listEqualTest(lixlist, stdlist);
}

TEST(ListTest,Func_sort) {
	lix::list<int> lixlist;
	std::list<int> stdlist;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);

	for (int i = 0; i<500; i++) {
		dice_roll = distribution(generator);
		lixlist.push_back(dice_roll);
		stdlist.push_back(dice_roll);
	}

	lixlist.sort();
	stdlist.sort();
	listEqualTest(lixlist, stdlist);
	//lix::list<int> list{ 7,3,23,7,45 };
	//list.sort();
}