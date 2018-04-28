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

	lix::vector<int> lixvec4{ 3,5,7,8 };
	std::vector<int> stdvec4{ 3,5,7,8 };
	vectorEqualTest(lixvec4, stdvec4);

	lix::vector<int> lixvec5(lixvec4.begin(), lixvec4.end());
	std::vector<int> stdvec5(stdvec4.begin(), stdvec4.end());
	vectorEqualTest(lixvec5, stdvec5);

	lix::vector<int> lixvec6 = std::move(lixvec4);
	std::vector<int> stdvec6 = std::move(stdvec4);
	vectorEqualTest(lixvec6, stdvec6);
	EXPECT_EQ(lixvec4.empty(), true);

	lix::vector<int> lixvec7(2);
	std::vector<int> stdvec7(2);
	vectorEqualTest(lixvec7, stdvec7);

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
		EXPECT_EQ(stdvec.at(i), lixvec.at(i));
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

TEST(VectorTest,operator_equal) {
	lix::vector<int> vec1(30, 5);

	lix::vector<int> vec2;
	vec2.push_back(3);
	vec2.push_back(6);
	vec2.push_back(9);
	vec1 = vec2;

	EXPECT_EQ(vec1[0], 3);
	EXPECT_EQ(vec1[1], 6);
	EXPECT_EQ(vec1[2], 9);
	EXPECT_EQ(vec1.size(), 3);
	EXPECT_EQ(vec2[0], 3);
	EXPECT_EQ(vec2[1], 6);
	EXPECT_EQ(vec2[2], 9);
	EXPECT_EQ(vec2.size(), 3);
	
	lix::vector<int> vec3;
	vec3.push_back(2);
	vec3.push_back(4);
	vec3.push_back(6);
	vec3.push_back(8);
	vec1 = std::move(vec3);

	EXPECT_EQ(vec1[0], 2);
	EXPECT_EQ(vec1[1], 4);
	EXPECT_EQ(vec1[2], 6);
	EXPECT_EQ(vec1.size(), 4);
	EXPECT_EQ(vec3.size(), 0);

	vec1 = { 1,2,3,4,5 };
	EXPECT_EQ(vec1[0], 1);
	EXPECT_EQ(vec1[1], 2);
	EXPECT_EQ(vec1[2], 3);
	EXPECT_EQ(vec1[3], 4);
	EXPECT_EQ(vec1[4], 5);
	EXPECT_EQ(vec1.size(), 5);
//	EXPECT_EQ(vec3.size(), 0);
}


TEST(VectorTest, Assign) {
	lix::vector<int> vec1;
	vec1.push_back(3);
	vec1.push_back(7);
	vec1.assign(3, 10);
	EXPECT_EQ(vec1[0], 10);
	EXPECT_EQ(vec1[1], 10);
	EXPECT_EQ(vec1[2], 10);
	EXPECT_EQ(vec1.size(), 3);

	lix::vector<int> vec2(6, 99);
	vec2.assign(vec1.begin(), vec1.end());
	EXPECT_EQ(vec2[0], 10);
	EXPECT_EQ(vec2[1], 10);
	EXPECT_EQ(vec2[2], 10);
	EXPECT_EQ(vec2.size(), 3);

	lix::vector<int> vec3(8, 34);
	vec3.assign({ 3,6,9 });
	EXPECT_EQ(vec3[0], 3);
	EXPECT_EQ(vec3[1], 6);
	EXPECT_EQ(vec3[2], 9);
	EXPECT_EQ(vec3.size(), 3);
}

TEST(VectorTest,Reserve) {
	lix::vector<int> lixvec{ 3,6,9 };
	EXPECT_EQ(lixvec.size(), 3);
	lixvec.reserve(100);
	EXPECT_EQ(lixvec[0], 3);
	EXPECT_EQ(lixvec[1], 6);
	EXPECT_EQ(lixvec[2], 9);
	EXPECT_EQ(lixvec.capacity(), 100);
}

TEST(VectorTest, Shrink) {
	lix::vector<int> lixvec{ 3,6,9 };
	EXPECT_EQ(lixvec.size(), 3);
	lixvec.reserve(10000000);
	lixvec.shrink_to_fit();
	EXPECT_EQ(lixvec[0], 3);
	EXPECT_EQ(lixvec[1], 6);
	EXPECT_EQ(lixvec[2], 9);
	EXPECT_EQ(lixvec.capacity(), lixvec.size());
}

TEST(VectorTest, Emplace) {
	struct test
	{
		test(int i):x(i+1){}
		int x;
	};
	test ex(3);
	lix::vector<test> lixvec(3, ex);
	EXPECT_EQ(lixvec[1].x, 4);
	lixvec.emplace(lixvec.begin() + 1, 5);
	EXPECT_EQ(lixvec[1].x, 6);
	lixvec.emplace_back(7);
	EXPECT_EQ(lixvec[3].x, 8);
	for (int i = 10; i < 50; ++i) lixvec.emplace_back(i);
	EXPECT_EQ(lixvec.back().x, 50);
}

TEST(VectorTest,Swap) {
	lix::vector<int> vec1{ 1,2,3 };
	lix::vector<int> vec2{ 3,6,9 };
	vec1.swap(vec2);
	EXPECT_EQ(vec1[0], 3);
	EXPECT_EQ(vec1[1], 6);
	EXPECT_EQ(vec1[2], 9);
	EXPECT_EQ(vec2[0], 1);
	EXPECT_EQ(vec2[1], 2);
	EXPECT_EQ(vec2[2], 3);
}

TEST(VectorTest,Resize) {
	lix::vector<int> vec1{ 1,2 };
	vec1.resize(10);
	EXPECT_EQ(vec1[0], 1);
	EXPECT_EQ(vec1[1], 2);
	for (int i = 2; i < 10; ++i) EXPECT_EQ(vec1[i], 0);
	EXPECT_EQ(vec1.size(), 10);
	EXPECT_EQ(vec1.capacity(), 10);

	for (int i = 0; i < 8; ++i) vec1.pop_back();
	EXPECT_EQ(vec1.size(), 2);
	EXPECT_EQ(vec1[0], 1);
	EXPECT_EQ(vec1[1], 2);

	vec1.resize(5);
	for (int i = 2; i < 5; ++i) EXPECT_EQ(vec1[i], 0);
	EXPECT_EQ(vec1.size(), 5);
	EXPECT_EQ(vec1.capacity(), 5);

	lix::vector<int> vec2{ 1,2,3,4,5,6,7,8,9 };
	vec2.resize(2);
	EXPECT_EQ(vec2[0], 1);
	EXPECT_EQ(vec2[1], 2);
	EXPECT_EQ(vec2.size(), 2);
	EXPECT_EQ(vec2.capacity(), 2);

	lix::vector<int> vec3{ 1,2 };
	vec3.resize(15,41);
	EXPECT_EQ(vec3[0], 1);
	EXPECT_EQ(vec3[1], 2);
	for (int i = 2; i < 10; ++i) EXPECT_EQ(vec3[i], 41);
	EXPECT_EQ(vec3.size(), 15);
	EXPECT_EQ(vec3.capacity(), 15);

	for (int i = 0; i < 13; ++i) vec3.pop_back();
	EXPECT_EQ(vec3.size(), 2);
	EXPECT_EQ(vec3[0], 1);
	EXPECT_EQ(vec3[1], 2);

	vec3.resize(5,55);
	for (int i = 2; i < 5; ++i) EXPECT_EQ(vec3[i], 55);
	EXPECT_EQ(vec3.size(), 5);
	EXPECT_EQ(vec3.capacity(), 5);
}

TEST(VectorTest, Clear) {
	lix::vector<int> vec(10, 10);
	vec.clear();
	EXPECT_EQ(vec.size(), 0);
	EXPECT_EQ(vec.capacity(), 10);
}