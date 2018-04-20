#ifndef VECTOR_H_
#define VECTOR_H_

#define _SCL_SECURE_NO_WARNINGS

#include "../include/memory"
#include <cstddef>
#include <algorithm>

namespace lix
{
	template<class T, class Alloc = _alloc_template>
	class vector
	{
	public:
		typedef T value_type;
		typedef value_type* ptr;
		typedef value_type* iterator;
		typedef value_type& ref;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		vector() :_start(nullptr), _end(nullptr), _tail(nullptr) {}
		vector(size_type n, const T&value) { fill_initialize(n, value); }
		vector(int n, const T& value) { fill_initialize(n, value); }
		vector(vector<T, Alloc>& vec);
		explicit vector(size_type n) { fill_initialize(n, T()); }
		~vector() {
			destroy(_start, _end);
			allocator::deallocate(_start, _tail - _start);
		}

		iterator begin() { return _start; }
		iterator end() { return _end; }
		size_type size() const { return static_cast<size_type>(_end - _start); }
		size_type capacity() const { return static_cast<size_type>(_tail - _start); }
		bool empty() { return _start == _end; }
		ref operator[](size_type n) { return *(_start + n); }
		vector<T, Alloc>& operator=(const vector<T, Alloc> vec);

		ref front() { return *_start; }
		ref back() { return *(_end - 1); }
		void push_back(const T& x);
		void pop_back();

		void insert(iterator pos, size_type n, const T& value);
		iterator erase(iterator first, iterator last);
		iterator erase(iterator pos);

	protected:
		void deallocate();
		iterator allocate_and_fill(size_type n, const T& x);
		void fill_initialize(size_type n, const T& value);

		void insert_aux(iterator pos, size_type n, const T& value);
		void insert_aux(iterator pos, const T& value);

	private:
		typedef simple_alloc<T, Alloc> allocator;
		iterator _start; //数据头
		iterator _end;   //数据尾
		iterator _tail;  //空间尾
	};


	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator pos) {
		if (pos + 1 != _end) {
			std::copy(pos + 1, _end, pos);
		}
		--_end;
		destroy(_end);
		return pos;
	}

	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
		iterator i = std::copy(last, _end, first);
		destroy(i, _end);
		_end = _end - (last - first);
		return first;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::insert(iterator pos, size_type n, const T& value) {
		if (n > 0) {
			if (static_cast<size_type>(_tail - _end) >= n) {
				auto elems_after = _end - pos;
				iterator old_end = _end;
			//	if (elems_after > n) {
					uninitialized_copy(_end - n, _end, _end);
					_end += n;
					std::copy_backward(pos, old_end - n, old_end);
					std::fill(pos, pos + n, value);
			//	}
			//	else {
		/*			uninitialized_copy(_end, n - elems_after, value);
					_end += n - elems_after;
					uninitialized_copy(pos, old_end, _end);
					_end += elems_after;
					std::fill(pos, old_end, value);
				}*/
			}
			else {
				insert_aux(pos, n, value);
			}
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::pop_back() {
		--_end;
		destroy(_end);
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::push_back(const T& x) {
		if (_end == _tail) {
			insert_aux(end(), x);
		}
		else {
			construct(_end, x);
			++_end;
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator pos, size_type n, const T& value) {
		const size_t old_size = size();
		const size_t new_size = old_size == 0 ? 1 : old_size * 2;
		auto new_start = allocator::allocate(new_size);
		auto new_end = uninitialized_copy(_start, pos, new_start);
		while (n--) {
			construct(new_end, value);
			++new_end;
		}
		new_end = uninitialized_copy(pos, _end, new_end);
		destroy(_start, _end);
		deallocate();
		_start = new_start;
		_end = new_end;
		_tail = _start + new_size;
	}

	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::allocate_and_fill(size_type n, const T& x) {
		iterator address = allocator::allocate(n);
		uninitialized_fill_n(address, n, x);
		return address;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::deallocate() {
		if (_start != nullptr) {
			allocator::deallocate(_start, _tail - _start);
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator pos, const T& value) {
		insert_aux(pos, 1, value);
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::fill_initialize(size_type n, const T& value) {
		_start = allocate_and_fill(n, value);
		_end = _start + n;
		_tail = _end;
	}

	template <class T, class Alloc>
	vector<T, Alloc>::vector(vector<T, Alloc>& vec) {
		_start = allocator::allocate(vec.size());
		_end = _start + vec.size();
		_tail = _end;
		auto tmp = _start;
		auto it = vec.begin();
		for (; it != vec.end(); ++it, ++tmp) construct(&*tmp, *it);
	}

	template <class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc> vec) {
		_start = allocator::allocate(vec.size());
		_end = _start + vec.size();
		_tail = _end;
		auto tmp = _start;
		auto it = vec.begin();
		for (; it != vec.end(); ++it, ++tmp) construct(&*tmp, *it);
		return *this;
	}

}



#endif
