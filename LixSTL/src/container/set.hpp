#ifndef SET_H_
#define SET_H_
#include "rb_tree.hpp"
#include <type_traits>

namespace lix
{
	template<
		class Key,
		class Compare = std::less<Key>,
		class Allocator = lix::allocator<Key>>
	class set
	{
		using key_type = Key;
		using value_type = Key;
		using key_compare = Compare;
		using value_compare = Compare;

		using rbtree = rb_tree<key_type, value_type, identity<value_type>, key_compare, Allocator>;
		rbtree t;

		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using allocator_type = Allocator;
		using reference = value_type & ;
		using const_reference = const reference;
		using pointer =typename allocator_traits<allocator_type>::pointer;
		using const_pointer = typename allocator_traits<allocator_type>::const_pointer;
		using iterator =typename rbtree::const_iterator;

		key_compare key_comp() const { return t.key_comp(); }
		value_compare value_comp() const { return t.key_comp(); }
		iterator begin() const { return t.begin(); }
		iterator end() const { return t.end(); }
		bool empty() const { return t.empty(); }
		size_type size() { return  t.size(); }

	};
}

#endif
