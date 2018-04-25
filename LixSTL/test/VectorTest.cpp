#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

#include <gtest/gtest.h>
#include "../include/vector"
#include <random>

template<class T>
void vectorEqualTest(lix::vector<T>& lixvec,std::vector<T>& stdvec) {
	auto lixitr = lixvec.begin();
	auto stditr = stdvec.begin();
	for (; lixitr != lixvec.end(); ++lixitr, ++stditr) {
		EXPECT_EQ(*lixitr, *stditr);
	}
}

TEST(VectorTest, Init) {
	lix::vector<std::string> lixvec1{};
	std::vector<std::string> stdvec1{};
	vectorEqualTest(lixvec1, stdvec1);

	lix::vector<std::string> lixvec2(3,"test");
    std::vector<std::string> stdvec2(3,"test");
	vectorEqualTest(lixvec2, stdvec2);

	lix::vector<std::string> lixvec3(lixvec2);
	std::vector<std::string> stdvec3(stdvec2);
	vectorEqualTest(lixvec3, stdvec3);
}

TEST(VectorTest, Push_and_Pop)
{
	std::vector<int> stdvec;
	lix::vector<int> lixvec;
	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);
	for (int i = 0; i < 50000;i++) {
		dice_roll = distribution(generator);
		stdvec.push_back(dice_roll);
		lixvec.push_back(dice_roll);
	}
	while(!stdvec.empty()) {
		EXPECT_EQ(stdvec.back(),lixvec.back());
		stdvec.pop_back();
		lixvec.pop_back();
	}
	EXPECT_EQ(lixvec.empty(), true);
}

TEST(VectorTest, ItrInit) {
	std::vector<int> stdvec;
	lix::vector<int> lixvec;
	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);
	for (int i = 0; i < 50000; i++) {
		dice_roll = distribution(generator);
		stdvec.push_back(dice_roll);
		lixvec.push_back(dice_roll);
	}
	std::vector<int> stdvec2(stdvec.begin(), stdvec.end());
	lix::vector<int> lixvec2(lixvec.begin(), lixvec.end());
	vectorEqualTest(lixvec2, stdvec2);

	std::vector<std::string> stdvecstr;
	lix::vector<std::string> lixvecstr;

	stdvecstr.push_back("test1");
	stdvecstr.push_back("vecstr");
	lixvecstr.push_back("test1");
	lixvecstr.push_back("vecstr");

	std::vector<std::string> stdvecstr2(stdvecstr.begin(), stdvecstr.end());
	lix::vector<std::string> lixvecstr2(lixvecstr.begin(), lixvecstr.end());
	vectorEqualTest(lixvec2, stdvec2);
}

TEST(VectorTest, Iterator)
{
	std::vector<int> stdvec;
	lix::vector<int> lixvec;
	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);
	for (int i = 0; i < 50000; i++) {
		dice_roll = distribution(generator);
		stdvec.push_back(dice_roll);
		lixvec.push_back(dice_roll);
	}
	lix::vector<int>::iterator lixitr = lixvec.begin();
	auto stditr = stdvec.begin();
	for(;lixitr!=lixvec.end();++lixitr,++stditr) {
		EXPECT_EQ(*lixitr, *stditr);
	}
}

TEST(VectorTest, SquareBracketOperator) {
	std::vector<int> stdvec;
	lix::vector<int> lixvec;
	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);
	for (int i = 0; i < 50000; i++) {
		dice_roll = distribution(generator);
		stdvec.push_back(dice_roll);
		lixvec.push_back(dice_roll);
	}
	for(size_t i=0;i<lixvec.size();++i) {
		EXPECT_EQ(stdvec[i], lixvec[i]);
	}
}

TEST(VectorTest, Erase) {
	std::vector<int> stdvec;
	lix::vector<int> lixvec;
	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);
	for (int i = 0; i < 50000; i++) {
		dice_roll = distribution(generator);
		stdvec.push_back(dice_roll);
		lixvec.push_back(dice_roll);
	}
	for(int i=0;i<10;i++) {
		std::uniform_int_distribution<size_t> idx(0, lixvec.size());
		size_t index = idx(generator);
		if(!lixvec.empty()) {
			lixvec.erase(lixvec.begin() + index);
			stdvec.erase(stdvec.begin() + index);
			vectorEqualTest(lixvec, stdvec);
		}
	}
	for (int i = 0; i<10; i++) {
		std::uniform_int_distribution<size_t> idx(0, lixvec.size());
		size_t index1 = idx(generator);
		size_t index2 = idx(generator);
		if(index1>index2) {
			auto tmp = index1;
			index1 = index2;
			index2 = tmp;
		}
		if (!lixvec.empty()) {
			lixvec.erase(lixvec.begin() + index1,lixvec.begin()+index2);
			stdvec.erase(stdvec.begin() + index1,stdvec.begin()+index2);
			vectorEqualTest(lixvec, stdvec);
		}
	}
}

TEST(VectorTest,Insert) {
	std::vector<int> stdvec;
	lix::vector<int> lixvec;
	std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));
	std::uniform_int_distribution<int> distribution(0, 10000);
	int dice_roll = distribution(generator);
	for (int i = 0; i < 50000; i++) {
		dice_roll = distribution(generator);
		stdvec.push_back(dice_roll);
		lixvec.push_back(dice_roll);
	}
	for(int i=0;i<10;i++) {
		int idx = distribution(generator);
		int n = distribution(generator);
		int val = distribution(generator);
		lixvec.insert(lixvec.begin() + idx, n, val);
		stdvec.insert(stdvec.begin() + idx, n, val);
		vectorEqualTest(lixvec, stdvec);
	}
}

TEST(VectorTest,Container) {
	lix::vector<std::string> vec;
	vec.push_back("asdf");
	vec.push_back("sadfqwer");
	vec.insert(vec.begin(), 1, "asdfasf");
}