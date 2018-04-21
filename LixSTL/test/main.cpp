//#include "../src/vector.hpp"
////#include <vector>
//#include <iostream>
////#include <list>
//#include "../src/list.hpp"
//#include "../src/deque.hpp"
//
//#pragma warning(disable: 4251)
//#pragma warning(disable: 4275)
//#include <gtest/gtest.h>
////#include "VectorTest.cpp"
//#include <memory>
//
//int main(int ac, char* av[])
//{
//	std::pointer_traits<int> it;
//	testing::InitGoogleTest(&ac, av);
//	RUN_ALL_TESTS();
//	system("pause");
//	return 0;
//}

#include "../src/memory/allocator.hpp"
#include "../src/memory/pointer_traits.hpp"
#include <iostream>
#include <memory>
#include "../src/memory/allocator_traits.hpp"
#include <vector>
using namespace lix;

template <class Ptr>
struct BlockList
{
	// 预定义内存块
	struct block;

	// 从指针类别 Ptr 定义指向内存块的指针
	// 若 Ptr 是任何 T* 类型，则 block_ptr_t 为 block*
	// 若 Ptr 是 smart_ptr<T> ，则 block_ptr_t 为 smart_ptr<block>
	typedef typename pointer_traits<Ptr>::template rebind<block> block_ptr_t;

	struct block
	{
		size_t size;
		block_ptr_t next_block;
	};

	block_ptr_t free_blocks;
};

int main()
{
	BlockList<int*> bl1;
	// bl1.free_blocks 的类型是 block*
//	std::allocator_traits<int>::construct();
	BlockList<std::shared_ptr<char>> bl2;
	allocator_traits<allocator<std::vector<int>>> it;
	// bl2.free_blocks 的类型是 std::shared_ptr<block>
	std::cout << bl2.free_blocks.use_count() << '\n';
	system("pause");
}