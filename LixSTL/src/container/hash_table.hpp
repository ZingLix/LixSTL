#ifndef HASHTABLE_H_
#define HASHTABLE_H_
#include "../memory/allocator.hpp"
#include "../memory/allocator_traits.hpp"
#include "../container/vector.hpp"

namespace lix {
	static const int _stl_num_primes = 28;
	static const size_t _stl_prime_list[_stl_num_primes] = {
	  53ul,         97ul,         193ul,        389ul,
	  769ul,        1543ul,       3079ul,       6151ul,      12289ul,
	  24593ul,      49157ul,      98317ul,      196613ul,    393241ul,
	  786433ul,     1572869ul,    3145739ul,    6291469ul,   12582917ul,
	  25165843ul,   50331653ul,   100663319ul,  201326611ul, 402653189ul,
	  805306457ul,  1610612741ul, 3221225473ul, 4294967291ul
	};

	inline size_t max_bucket_count() {
		return _stl_prime_list[_stl_num_primes - 1];
	}

	inline size_t _stl_next_prime(size_t n) {
		for (int i = 0; i < _stl_num_primes; ++i) {
			if (_stl_prime_list[i] >= n) return _stl_prime_list[i];
		}
		return max_bucket_count();
	}

	template<class Value>
	struct _hash_table_node
	{
		Value val;
		_hash_table_node* next;

		_hash_table_node(Value v):val(v),next(nullptr){}
	};

	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	struct _hash_table_iterator;

	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc = allocator<Value>>
	class hash_table
	{
	public:
		using hasher = HashFunc;
		using key_equal = EqualKey;
		using size_type = size_t;

		using value_type = Value;
		using key_type = Key;
		using iterator = _hash_table_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;
	private:
		

		using node = _hash_table_node<Value>;
		using node_allocator = typename lix::allocator_traits<Alloc>::template rebind_alloc<node>;

	protected:
		node* new_node(const value_type& obj) {
			node* n = node_allocator::allocate(1);
			n->next = nullptr;
			allocator_traits<node_allocator>::construct(alloc_, n, obj);
			return n;
		}

		void delete_node(node* n) {
			allocator_traits<node_allocator>::destroy(alloc_, n);
			node_allocator::deallocate(n);
		}

		static size_type next_size(size_type n) {
			return _stl_next_prime(n);
		}

		void initialize_buckets(size_type n) {
			buckets.reserve(next_size(n));
			buckets.insert(buckets.end(), buckets.capacity(), nullptr);
		}

		void resize(size_type num_elements_hint) {
			const size_type old_n = buckets.size();
			if(num_elements_hint>old_n) {
				const size_type n = next_size(num_elements_hint);
				if(n>old_n) {
					std::vector<node*> tmp(n, nullptr);
					for(size_type bucket=0;bucket<old_n;++bucket) {
						node* first = buckets[bucket];
						while(first!=nullptr) {
							size_type new_bucket = bkt_num(first->val, n);
							buckets[bucket] = first->next;
							first->next = tmp[new_bucket];
							tmp[new_bucket] = first;
							first = buckets[bucket];
						}
					}
					buckets.swap(tmp);
				}
			}
		}

	public:
		size_type bkt_num(const value_type& obj, size_t n) const
		{
			return bkt_num_key(get_key_(obj), n);
		}

		size_type bkt_num(const value_type& obj) const
		{
			return bkt_num_key(get_key_(obj));
		}

		size_type bkt_num_key(const key_type& key) const
		{
			return bkt_num_key(key, buckets.size());
		}

		size_type bkt_num_key(const key_type& key, size_t n) const
		{
			return hash_(key) % n;
		}


		hash_table(size_type n,const HashFunc& func,const EqualKey& eql) 
			:hash_(func),equal_(eql),get_key_(ExtractKey()),num_elements(0),alloc_(){
			initialize_buckets(n);
		}

		std::pair<iterator,bool> insert_unique(const value_type& val) {
			resize(num_elements + 1);
			return insert_unique_aux(val);
		}

		std::pair<iterator, bool> insert_unique_aux(const value_type& val) {
			const size_type n = bkt_num(val);
			node* first = buckets[n];

			for (node* cur = first; cur; cur = cur->next) {
				if (equal_(
					get_key_(cur->val), 
					get_key_(val))) {
					return std::pair<iterator, bool>(iterator(cur, this), false);
				}
			}
			node* tmp = new_node(val);
			tmp->next = first;
			buckets[n] = tmp;
			++num_elements;
			return std::pair<iterator, bool>(iterator(tmp, this), true);
		}

		std::pair<iterator, bool> insert_equal(const value_type& val) {
			resize(num_elements + 1);
			return insert_equal_aux(val);
		}

		std::pair<iterator, bool> insert_equal_aux(const value_type& val) {
			const size_type n = bkt_num(val);
			node* first = buckets[n];

			for (node* cur = first; cur; cur = cur->next) {
				if (equals(get_key(cur->val), get_key_(val))) {
					node* tmp = new_node(val);
					tmp->next = cur->next;
					cur->next = tmp;
					++num_elements;
					return iterator(tmp, this);
				}
			}
			node* tmp = new_node(val);
			tmp->next = first;
			buckets[n] = tmp;
			++num_elements;
			return iterator(tmp, this);
		}

		void clear() {
			for (size_type i = 0; i < buckets.size(); ++i) {
				node* cur = buckets[i];
				while (cur != nullptr) {
					node* next = cur->next;
					delete_node(cur);
					cur = next;
				}
				buckets[i] = 0;
			}
			num_elements = 0;
		}

		void copy_from(const hash_table& ht) {
			buckets.clear();
			buckets.reserve(ht.buckets.size());
			buckets.insert(buckets.end(), ht.buckets.size(),nullptr);
			for (size_type i = 0; i < ht.buckets.size(); ++i) {
				if (const node* cur = ht.buckets[i]) {
					node* copy = new_node(cur->val);
					buckets[i] = copy;

					for (node* next = cur->next; next; cur = next, next = cur->next) {
						copy->next = new_node(next->val);
						copy = copy->next;
					}
				}
				num_elements = ht.num_elements;
			}
		}

		size_type bucket_count() const { return buckets.size(); }
		size_type size() const { return num_elements; }
		bool empty() const { return size() == 0; }

		iterator begin() {
			for(auto it=buckets.begin();it!=buckets.end();++it) {
				if (*it != nullptr) return iterator{ *it, this };
			}
			return iterator(nullptr, this);
		}

		iterator end() {
			iterator it{ nullptr, this };
			return it;
		}

		node* next_bkt(const value_type& val) {
			auto n = bkt_num(val);
			for(++n;n<bucket_count();++n) {
				if (buckets[n] != nullptr) return buckets[n];
			}
			return nullptr;
		}

		iterator find(const value_type& val) {
			node* ptr = buckets[bkt_num(val)];
			while(ptr!=nullptr) {
				if (ptr->val == val) return iterator(ptr, this);
				ptr = ptr->next;
			}
			return iterator(nullptr, this);
		}

		iterator find_by_key(const key_type& val) {
			node* ptr = buckets[bkt_num_key(val)];
			return iterator(ptr, this);
		}

		~hash_table() {
			clear();
		}

	private:
		hasher hash_;
		key_equal equal_;
		ExtractKey get_key_;
		std::vector<node*> buckets;
		size_type num_elements;
		node_allocator alloc_;
	};

	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	struct _hash_table_iterator
	{
		using node = _hash_table_node<Value>;
		using hashtable = hash_table<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;
		using iterator = _hash_table_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;

		using iterator_category = forward_iterator_tag;
		using value_type = Value;
		using difference_type = ptrdiff_t;
		using size_type = size_t;
		using reference = value_type & ;
		using pointer = value_type * ;

		node* cur;
		hashtable* ht;
		_hash_table_iterator(node* nodeptr, hashtable* htptr) :cur(nodeptr), ht(htptr) {}

		reference operator*() const { return cur->val; }
		pointer operator->() const { return &(operator*()); }
		iterator& operator++() {
			node* old = cur;
			cur = cur->next;
			if (cur == nullptr) {
				cur = ht->next_bkt(old->val);
			}
			return *this;
		}
		iterator operator++(int) {
			iterator tmp = *this;
			++*this;
			return tmp;
		}
		bool operator==(const iterator& it) const { return cur == it.cur; }
		bool operator!=(const iterator& it) const { return cur != it.cur; }
	};
}


#endif