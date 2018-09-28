#ifndef UNORDERED_SET_H_
#define UNORDERED_SET_H_

#include "../memory/allocator.hpp"
#include "../algorithm/algorithm.hpp"
#include "hash_table.hpp"

namespace lix
{
	template<class Key,class Hash = std::hash<Key>,
		class KeyEqual = std::equal_to<Key>, class Allocator = allocator<Key>> 
	class unordered_set
	{
	private:
		using hashtable = hash_table<Key, Key, Hash, identity<Key>, KeyEqual, Allocator>;

	public:
		using key_type = typename hashtable::key_type;
		using value_type = typename hashtable::value_type;
		using hasher = typename hashtable::hasher;
		using key_equal = typename hashtable::key_equal;

		using size_type = typename hashtable::size_type;
		using iterator = typename hashtable::iterator;

		unordered_set():ht(100,hasher(),key_equal()){}
		explicit unordered_set(size_type n):ht(n,hasher(),key_equal()){}

		size_type size() const { return ht.size(); }
		bool empty() const { return ht.empty(); }

		iterator begin() const { return ht.begin(); }
		iterator end()  { return ht.end(); }

		std::pair<iterator,bool> insert(const value_type& val) {
			return ht.insert_unique(val);
		}

		iterator find(const value_type& val) { return ht.find(val); }

	private:
		hashtable ht;
	};
}





#endif
