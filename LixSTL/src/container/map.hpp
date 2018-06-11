#ifndef MAP_H_
#define MAP_H_
#include <xstddef>
#include <utility>
#include "../memory/allocator.hpp"
#include "pair.hpp"
#include "../memory/allocator_traits.hpp"
#include "rb_tree.hpp"

namespace lix
{
	template<class Key,class T,
		class Compare = std::less<Key>,
		class Allocator = allocator<std::pair<const Key, T>>>
	class map
	{

	public:
		using key_type = Key;
		using mapped_type = T;
		using value_type = std::pair<const Key, T>;
		using key_compare = Compare;

	private:
		using pair = std::pair<key_type, mapped_type>;
		struct _comp
		{
			_comp():comp(){}
			_comp(Compare compare):comp(compare){}
			key_compare comp;
			bool operator()(pair p1,pair p2){
				return comp(p1.first, p2.first);
			}
		};
		using _key_compare = _comp;
		using rbtree = rb_tree<pair, _key_compare, Allocator>;
		
		rbtree t;

	public:
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		
		using allocator_type = Allocator;
		using reference = value_type & ;
		using const_reference = const value_type&;
		using pointer =typename allocator_traits<Allocator>::pointer;
		using const_pointer = typename allocator_traits<Allocator>::const_pointer;
		using iterator =typename rbtree::iterator;
		using const_iterator = typename rbtree::const_iterator;

		map():t(){}
		explicit map(const Compare& comp,
			const Allocator& alloc = Allocator()):t(comp,alloc){}
		template< class InputIterator >
		map(InputIterator first, InputIterator last,
			const Compare& comp = Compare(),
			const Allocator& alloc = Allocator()):t(first,last,true,_key_compare(comp),alloc){}
		template< class InputIterator >
		map(InputIterator first, InputIterator last,
			const Allocator& alloc):t(first,last,true,_key_compare(),alloc){}
		map(map& other):t(other.t){}
		map(std::initializer_list<pair> init,
			const Compare& comp = Compare(),
			const Allocator& alloc = Allocator()):t(init,true,_key_compare(),alloc){}

		~map(){}
		iterator begin() { return t.begin(); }
		iterator end() { return t.end(); }
		bool empty() { return t.empty(); }
		size_type size() { return t.size(); }

		std::pair<iterator, bool> insert(const value_type& value) {
			return t.insert_unique(value);
		}
		template< class InputIt >
		void insert(InputIt first, InputIt last) {
			return t.insert_unique(first, last);
		}
		void insert(std::initializer_list<value_type> ilist) {
			t.insert_unique(ilist);
		}

		iterator erase(iterator pos) {
			return t.erase(pos);
		}
		iterator erase(const_iterator first, const_iterator last) {
			return t.erase(first, last);
		}
		size_type erase(const key_type& key) {
		//	TODO unnecessary construct
			return t.erase(value_type(key, T()));
		}

		iterator find(const Key& key) {
			return t.find(key);
		}

		T& operator[](const key_type& key)
		{
			return (*(insert(std::make_pair(key, T())).first)).second;
		}
	};
}

#endif
