#ifndef VECTOR_H_
#define VECTOR

#include "../include/memory"
#include <cstddef>

namespace lix
{
template<class T,class Alloc=_alloc_template>
class vector
{
public:
	typedef T value_type;
	typedef value_type* ptr;
	typedef value_type* iterator;
	typedef value_type& ref;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

protected:
	typedef simple_alloc<T, Alloc> allocator;
	iterator _start; //数据头
	iterator _end;   //数据尾
	iterator _tail;  //空间尾

	void deallocate() {
		if(_start!=nullptr) {
			allocator::deallocate(_start, _tail - _start);
		}
	}

	iterator allocate_and_fill(size_type n, const T& x) {
		iterator address = allocator::allocate(n);
		uninitialized_fill_n(address, n, x);
		return address;
	}

	void fill_initialize(size_type n,const T& value) {
		_start = allocate_and_fill(n, value);
		_end = _start + n;
		_tail = _end;
	}

	void expand();

public:
	iterator begin() { return _start; }
	iterator end() { return _end; }
	size_type size() const { return static_cast<size_type>(_end - _start); }
	size_type capacity() const { return static_cast<size_type>(_tail - _start); }
	bool empty() { return _start == _end; }
	ref operator[](size_type n) { return *(_start + n); }

	vector():_start(nullptr),_end(nullptr),_tail(nullptr){}
	vector(size_type n,const T&value) {
		fill_initialize(n, value);
	}
	vector(int n,const T& value) {
		fill_initialize(n, value);
	}
	explicit vector(size_type n) {
		fill_initialize(n, T());
	}

	~vector() {
		destroy(_start, _end);
	}

	ref front() { return *_start; }
	ref back() { return *(_end - 1); }
	void push_back(const T& x) {
		if(_end==_tail) {
			expand();
		} 
		construct(_end, x);
		++_end;
	}
	void pop_back() {
		--_end;
		destroy(_end);
	}

	iterator erase(iterator pos) {
		if(pos+1!=_end) {
			copy(pos + 1, _end, pos);
		}
		--_end;
		destroy(_end);
		return pos;
	}
};


}



#endif
