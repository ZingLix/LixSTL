#ifndef DEQUE_H_
#define DEQUE_H_
#include "../traits/iterator_traits.hpp"
#include "../../include/memory"
#include "../algorithm/algorithm.hpp"
#include <algorithm>

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
		typedef value_type* pointer;
		typedef value_type& reference;
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

		reference operator*()const { return *cur; }
		pointer operator->() const { return &(operator*()); }

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
		self operator-=(difference_type n)  { return (*this).operator+=(-n); }
		self operator-(difference_type n)const {
			self tmp = *this;
			return tmp -= n;
		}
		reference operator[](difference_type n) const { return *(*this + n); }
		
		bool operator==(const self& x) { return cur == x.cur; }
		bool operator!=(const self&x) { return !(*this == x); }
		bool operator<(const self& x) { return (node == x.node) ? (cur < x.cur) : (node < x.node); }
	};


	template <class T,class Alloc=allocator<T>,size_t Bufsiz=0>
	class deque
	{
	public:
		using value_type = T;
		using allocator_type = Alloc;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type & ;
		using const_reference = const reference;
		using pointer =typename allocator_traits<Alloc>::pointer;
		using const_pointer = const pointer;
		using iterator = __deque_iterator<T, T&, T*, Bufsiz>;
		using const_iterator = const iterator;

	protected:
		using map_pointer = pointer * ;
		using data_allocator = Alloc;
		using map_allocator = typename allocator_traits<Alloc>::template rebind_alloc<pointer>;
		
		const static  size_type INI_MAP_SIZE = 8;


	public:
		deque(int n, const value_type& val) :start(), finish(), map(nullptr), map_size(0),alloc_() {
			fill_initialize(n, val);
		}
		deque() :start(), finish(), map(nullptr), map_size(0),alloc_() {
			fill_initialize(0, value_type());
		}
		~deque() {
			clear();
		}

		reference front() { return *start; }
		reference back() {
			//iterator tmp = finish;
			//--tmp;
			//return *tmp;
			// TODO ????
			return *(finish - 1);
		}
		iterator begin() { return start; }
		iterator end() { return finish; }

		bool empty() { return finish == start; }
		size_type size() { return finish - start; }
		size_type max_size() { return size_type(-1); } //????????

		void clear() {
			for (map_pointer node = start.node + 1; node<finish.node; ++node) {
				auto p = *node;
				for (; p != *node + buffer_size(); ++p) {
					allocator_traits<Alloc>::destroy(alloc_, p);
				}
				data_allocator::deallocate(*node, buffer_size());
			}
			if (start.node != finish.node) {
				auto p = start.cur;
				for (; p != start.last; ++p) {
					allocator_traits<Alloc>::destroy(alloc_, p);
				}
				p = finish.cur;
				for (; p != finish.last; ++p) {
					allocator_traits<Alloc>::destroy(alloc_, p);
				}
				data_allocator::deallocate(finish.first, buffer_size());
			}
			else {
				auto p = start.cur;
				for (; p != finish.cur; ++p) {
					allocator_traits<Alloc>::destroy(alloc_, p);
				}
				//destroy(start.cur, finish.cur);
			}
			finish = start;
		}
		void pop_back() {
			if (finish.cur != finish.first) {
				--finish.cur;
				allocator_traits<Alloc>::destroy(alloc_, finish.cur);
				//destroy(finish.cur);
			}
			else {
				pop_back_aux();
			}
		}
		void push_back(const value_type& t) {
			if (finish.cur != finish.last - 1) {
				allocator_traits<Alloc>::construct(alloc_, finish.cur, t);
				//construct(finish.cur, t);
				++finish.cur;
			}
			else {
				push_back_aux(t);
			}
		}
		void pop_front() {
			if (start.cur != start.last - 1) {
				allocator_traits<Alloc>::destroy(alloc_, start.cur);
				//destroy(start.cur);
				++start.cur;
			}
			else {
				pop_front_aux();
			}
		}
		void push_front(const value_type& t) {
			if (start.cur != start.first) {
				allocator_traits<Alloc>::construct(alloc_, start.cur - 1, t);
				--start.cur;
			}
			else {
				push_front_aux(t);
			}
		}


	protected:
		static size_type buffer_size() { return _deque_buf_size(Bufsiz, sizeof(T)); }
		static size_type initial_map_size() { return INI_MAP_SIZE; }

		pointer allocate_node() {
			return data_allocator::allocate(buffer_size());
		}

		void deallocate_node(pointer itr) {
			data_allocator::deallocate(itr);
		}

		void create_map_and_nodes(size_type elements_num) {
			const size_type nodes_num = elements_num / buffer_size() + 1;
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

		reference operator[](size_type n)
		{
			return start[difference_type(n)];
		}

		void reallocate_map(size_type new_nodes_num,bool add_front) {
			auto old_num_nodes = finish.node - start.node + 1;
			auto new_num_nodes = old_num_nodes + new_nodes_num;
			map_pointer new_nstart;
			if(map_size>2*new_num_nodes) {
				new_nstart = map + (max_size() - new_num_nodes) / 2 + (add_front ? new_nodes_num : 0);
				if (new_nstart < start.node) {
					std::copy(start.node, finish.node + 1, new_nstart);
				} else {
					std::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
				}
			} else {
				size_type new_map_size = map_size + max(map_size, new_nodes_num) + 2;
				const map_pointer new_map = map_allocator::allocate(new_map_size);
				new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_front ? new_nodes_num : 0);
				std::copy(start.node, finish.node + 1, new_nstart);
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
			allocator_traits<Alloc>::construct(alloc_, finish.cur, t_value);
			//construct(finish.cur, t_value);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
		}
		void push_front_aux(const value_type& t) {
			value_type t_copy = t;
			reserve_map_at_front();
			*(start.node - 1) = allocate_node();
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			allocator_traits<Alloc>::construct(alloc_, start.cur, t_copy);
			//construct(start.cur, t_copy);
		}

		void pop_back_aux() {
			deallocate_node(finish.first);
			finish.set_node(finish.node - 1);
			finish.cur = finish.last - 1;
			allocator_traits<Alloc>::destroy(alloc_, finish.cur);
			//destroy(finish.cur);
		}
		void pop_front_aux() {
			allocator_traits<Alloc>::destroy(alloc_, start.cur);
			//destroy(start.cur);
			deallocate_node(start.first);
			start.set_node(start.node +1);
			start.cur = start.first;
		}

		iterator start;
		iterator finish;
		map_pointer map;
		size_type map_size;
		Alloc alloc_;
	};










}


#endif
