#ifndef DEQUE_H_
#define DEQUE_H_
#include "traits.hpp"
#include "memory_alloc.hpp"
#include "memory_uninitialized.hpp"
#include "algorithm.hpp"

namespace lix
{
	inline size_t _deque_buf_size(size_t n, size_t sz) {
		return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
	}

	template<class T, class Ref, class Ptr, size_t Bufsiz>
	struct __deque_iterator
	{
		typedef __deque_iterator<T, T&, T*, Bufsiz> iterator;
		typedef __deque_iterator<T, const T&, const T*, Bufsiz> const_iterator;

		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef value_type* ptr;
		typedef value_type& ref;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T** map_pointer;

		typedef __deque_iterator self;

		T* cur;
		T* first;
		T* last;
		map_pointer node;

		static size_type buffer_size() { return _deque_buf_size(Bufsiz, sizeof(T)); }

		void set_node(map_pointer new_node) {
			node = new_node;
			first = *node;
			last = first + difference_type(buffer_size());
		}

		ref operator*()const { return *cur; }
		ptr operator->() const { return &(operator*()); }

		difference_type operator-(const self& x) {
			return difference_type(buffer_size())*(node - x.node - 1) + (cur - first) + (x.last - x.cur);
		}
		self& operator++() {
			++cur;
			if(cur==last) {
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}
		self operator++(int) {
			const self tmp = *this;
			++*this;
			return tmp;
		}
		self& operator--() {
			if(cur==first) {
				set_node(node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}
		self operator--(int) {
			const self tmp = *this;
			--*this;
			return tmp;
		}
		self& operator+=(difference_type n) {
			difference_type offset = n + (cur - first);
			if (offset >= 0 && offset < difference_type(buffer_size())) {
				cur += n;
			}else {
				difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size()) : -difference_type((-offset - 1) / buffer_size()) - 1;
				set_node(node + node_offset);
				cur = first + (offset - node_offset * difference_type(buffer_size()));
			}
			return *this;
		}
		self operator+(difference_type n) const {
			self tmp = *this;
			return tmp += n;
		}
		self operator-=(difference_type n) const { return *this -= -n; }
		self operator-(difference_type n)const {
			self tmp = *this;
			return tmp -= n;
		}
		ref operator[](difference_type n) const { return *(*this + n); }

		bool operator==(const self& x) { return cur == x.cur; }
		bool operator!=(const self&x) { return !(*this == x); }
		bool operator<(const self& x) { return (node == x.node) ? (cur < x.cur) : (node < x.node); }
	};


	template <class T,class Alloc=_alloc_template,size_t Bufsiz=0>
	class deque
	{
	public:
		typedef T value_type;
		typedef value_type* ptr;
		typedef value_type& ref;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef __deque_iterator<T, T&, T*, Bufsiz> iterator;

	protected:
		typedef ptr* map_pointer;

		typedef simple_alloc<value_type, Alloc> data_allocator;
		typedef simple_alloc<ptr, Alloc> map_allocator;
		
		iterator start;
		iterator finish;
		map_pointer map;
		size_type map_size;

		const static  size_type INI_MAP_SIZE = 8;

	public:
		static size_type buffer_size() { return _deque_buf_size(Bufsiz, sizeof(T)); }
		static size_type initial_map_size() { return INI_MAP_SIZE; }

		ptr allocate_node() {
			return data_allocator::allocate(buffer_size());
		}

		void create_map_and_nodes(size_type elements_num) {
			size_type nodes_num = elements_num / buffer_size() + 1;
			map_size = initial_map_size() < nodes_num ? nodes_num + 2 : initial_map_size();
			map = map_allocator::allocate(map_size);
			map_pointer nstart = map + (map_size - nodes_num) / 2;
			map_pointer nfinish = nstart + nodes_num - 1;
			for(map_pointer cur = nstart;cur<=nfinish;++cur) {
				*cur = allocate_node();
			}
			start.set_node(nstart);
			finish.set_node(nfinish);
			start.cur = start.first;
			finish.cur = finish.first + elements_num % buffer_size();
		}

		void fill_initialize(size_type n,const value_type& val) {
			create_map_and_nodes(n);
			for (map_pointer cur = start.node; cur < finish.node; ++cur) {
				uninitialized_fill(*cur, *cur + buffer_size(), val);
			}
			uninitialized_fill(finish.first, finish.cur, val);
		}

		deque(int n,const value_type& val):start(),finish(),map(nullptr),map_size(0) {
			fill_initialize(n, val);
		}

		iterator begin() { return start; }
		iterator end() { return finish; }

		ref operator[](size_type n)
		{
			return start[difference_type(n)];
		}

		ref front() { return *start; }
		ref back() {
			//iterator tmp = finish;
			//--tmp;
			//return *tmp;
			// TODO ????
			return *(finish - 1);
		}
		size_type size() { return finish - start; }
		size_type max_size() { return size_type(-1); } //????????
		bool empty() { return finish == start; }

		void reallocate_map(size_type new_nodes_num,bool add_front) {
			size_type old_num_nodes = finish.node - start.node + 1;
			size_type new_num_nodes = old_num_nodes + new_nodes_num;
			map_pointer new_nstart;
			if(map_size>2*new_num_nodes) {
				new_nstart = map + (max_size() - new_num_nodes) / 2 + (add_front ? new_nodes_num : 0);
				if (new_nstart < start.node) {
					copy(start.node, finish.node + 1, new_nstart);
				} else {
					std::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
				}
			} else {
				size_type new_map_size = map_size + max(map_size, new_nodes_num) + 2;
				map_pointer new_map = map_allocator::allocate(new_map_size);
				new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_front ? new_nodes_num : 0);
				copy(start.node, finish.node + 1, new_nstart);
				map_allocator::deallocate(map, map_size);
				map = new_map;
				map_size = new_map_size;
			}
			start.set_node(new_nstart);
			finish.set_node(new_nstart + old_num_nodes - 1);
		}
		void reserve_map_at_back(size_type new_nodes_num=1) {
			if (new_nodes_num + 1 > map_size - (finish.node - map)) reallocate_map(new_nodes_num, false);
		}
		void reserve_map_at_front(size_type new_nodes_num = 1) {
			if (new_nodes_num > static_cast<size_type>(start.node-map)) reallocate_map(new_nodes_num, false);
		}

		void push_back_aux(const value_type& t) {
			value_type t_value = t;
			reserve_map_at_back();
			*(finish.node + 1) = allocate_node();
			construct(finish.cur, t_value);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
		}
		void push_back(const value_type& t) {
			if (finish.cur != finish.last - 1) {
				construct(finish.cur, t);
				++finish.cur;
			}
			else {
				push_back_aux(t);
			}
		}
		void push_front_aux(const value_type& t) {
			value_type t_copy = t;
			reserve_map_at_front();
			*(start.node - 1) = allocate_node();
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			construct(start.cur, t_copy);
		}
		void push_front(const value_type& t) {
			if (start.cur != start.first) {
				construct(start.cur - 1, t);
				--start.cur;
			}
			else {
				push_front_aux(t);
			}
		}
	};










}


#endif