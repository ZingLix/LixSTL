#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

#include <gtest/gtest.h>

#include <random>
#include "../src/container/map.hpp"
#include <map>

template<class T1,class T2>
void maptest(lix::map<T1,T2>& lixmap,std::map<T1,T2>& stdmap) {
	auto lixit = lixmap.begin();
	auto stdit = stdmap.begin();
	for (; stdit != stdmap.end(); ++lixit, ++stdit) {
		EXPECT_EQ((*lixit).first, (*stdit).first);
		EXPECT_EQ((*lixit).second, (*stdit).second);
	}
	EXPECT_EQ(lixmap.size(), stdmap.size());
	EXPECT_EQ(lixit == lixmap.end(), true);
}

TEST(MapTest, Init) {
	lix::map<int, int> lixmap({ { 3,5 },{ 4,1 },{ 6,2 } });
	std::map<int, int> stdmap({ { 3,5 },{ 4,1 },{ 6,2 } });
	maptest(lixmap, stdmap);

	lix::map<int, int> lixmap1(lixmap.begin(), lixmap.end());
	maptest(lixmap1, stdmap);

	lix::map<int, int> lixmap2(lixmap);
	maptest(lixmap2, stdmap);
}

TEST(MapTest, insert_and_erase) {
	lix::map<int,int> lixmap;
	std::map<int,int> stdmap;

	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll1,dice_roll2;

	for (int i = 0; i<1000; ++i) {
		dice_roll1 = distribution(generator);
		dice_roll2 = distribution(generator);
		lixmap.insert(std::make_pair(dice_roll1,dice_roll2));
		stdmap.insert(std::make_pair(dice_roll1, dice_roll2));
	}
	maptest(lixmap, stdmap);

	std::uniform_int_distribution<int> distribution1(1000, 1100);
	for (int i = 0; i<1000; ++i) {
		dice_roll1 = distribution(generator);
		dice_roll2 = distribution(generator);
		lixmap.insert(std::make_pair(dice_roll1, dice_roll2));
		stdmap.insert(std::make_pair(dice_roll1, dice_roll2));
	}
	maptest(lixmap, stdmap);

	for (int i = 0; i<10005; ++i) {
		EXPECT_EQ(lixmap.erase(i), stdmap.erase(i));
	}
	maptest(lixmap, stdmap);
	EXPECT_EQ(lixmap.empty(), true);

	for (int i = 0; i<1000; ++i) {
		dice_roll1 = distribution(generator);
		dice_roll2 = distribution(generator);
		lixmap.insert(std::make_pair(dice_roll1, dice_roll2));
		stdmap.insert(std::make_pair(dice_roll1, dice_roll2));
	}
	maptest(lixmap, stdmap);
}
