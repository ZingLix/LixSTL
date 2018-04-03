#ifndef MEMORY_CONSTRUCT_H_
#define MEMORY_CONSTRUCT_H_

#include <new>
#include "traits.hpp"

namespace lix
{

template<class T1, class T2>
void construct(T1* p, const T2& value) {
	new (p) T1(value);
}

template<class T>
void destroy(T* ptr) {
	ptr->~T();
}

template<class ForwardIterator, class T>
void _destroy(ForwardIterator first, ForwardIterator last, T*) {
	while (first <= last) {
		destroy(&*first);
		++first;
	}
}

template<class ForwardIterator>
void destroy(ForwardIterator first, ForwardIterator last) {
	_destroy(first, last, value_type(first));
}

template<class ForwardIterator>
void copy(ForwardIterator first,ForwardIterator end,ForwardIterator new_pos) {
	while(first<=end) {
		*new_pos = *first;
	}
}


}

#endif
