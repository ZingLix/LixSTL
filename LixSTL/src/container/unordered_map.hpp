#ifndef UNORDERED_MAP_H_
#define UNORDERED_MAP_H_

#include "../memory/allocator.hpp"
#include "../algorithm/algorithm.hpp"
#include "hash_table.hpp"

namespace lix
{
	template<class Key, class T, class Hash = std::hash<Key>,
		class KeyEqual = std::equal_to<Key>, class Allocator = allocator<Key>>
		class unordered_map
	{
	private:
		using hashtable = hash_table<std::pair<Key,T>, Key, Hash, select1st<Key,T>, KeyEqual, Allocator>;

	public:
		using key_type = typename hashtable::key_type;
		using value_type = typename hashtable::value_type;
		using hasher = typename hashtable::hasher;
		using key_equal = typename hashtable::key_equal;

		using size_type = typename hashtable::size_type;
		using iterator = typename hashtable::iterator;

		unordered_map() :ht(100, hasher(), key_equal()) {}
		explicit unordered_map(size_type n) :ht(n, hasher(), key_equal()) {}

		size_type size() const { return ht.size(); }
		bool empty() const { return ht.empty(); }

		iterator begin() const { return ht.begin(); }
		iterator end() { return ht.end(); }

		std::pair<iterator, bool> insert(const value_type& val) {
			return ht.insert_unique(val);
		}

		iterator find(const Key& val) { return ht.find_by_key(val); }

	private:
		hashtable ht;
	};
}





#endif
