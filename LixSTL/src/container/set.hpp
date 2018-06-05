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
	protected:
		using rbtree = rb_tree<key_type, key_compare, Allocator>;
		rbtree t;
	public:
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using allocator_type = Allocator;
		using reference = key_type & ;
		using const_reference = const reference;
		using pointer =typename allocator_traits<allocator_type>::pointer;
		using const_pointer = typename allocator_traits<allocator_type>::const_pointer;
		using iterator =typename rbtree::/*const_*/iterator;
		using const_iterator = iterator;
	
		set():t(){}
		explicit set(const Compare& comp,
			const Allocator& alloc = Allocator()):t(comp,alloc) {}

		template< class InputIt >
		set(InputIt first, InputIt last,
			const Compare& comp = Compare(),
			const Allocator& alloc = Allocator()):t(first,last,true,comp,alloc){}

		set(set& other) :t(other.t){}

		set(std::initializer_list<value_type> init,
			const Compare& comp = Compare(),
			const Allocator& alloc = Allocator()):t(init,true,comp,alloc){}

		set& operator=(const set& other) {
			t = other.t;
			return *this;
		}

		set& operator=(std::initializer_list<value_type> ilist) {
			t.clear();
			t.insert_unique(ilist);
			return *this;
		}

		key_compare key_comp() const { return t.key_comp(); }
		iterator begin()  { return t.begin(); }
		iterator end()  { return t.end(); }
		bool empty() const { return t.empty(); }
		size_type size() { return  t.size(); }
		allocator_type get_allocator() const { return t.get_allocator(); }
		using pair = std::pair<iterator, bool>;

		pair insert(const value_type& val) {
			return t.insert_unique(val);
		}
		template< class InputIt >
		void insert(InputIt first, InputIt last) {
			t.insert_unique(first, last);
		}
		void insert(std::initializer_list<value_type> ilist) {
			t.insert_unique(ilist);
		}
		iterator erase(const_iterator pos) {
			return t.erase(pos);
		}
		iterator erase(const_iterator first, const_iterator last) {
			return t.erase(first, last);
		}
		size_type erase(const key_type& key) {
			return t.erase(key);
		}
		void print() { t.print(); }
	};
}

#endif
