#ifndef MEMORY_UNINITIALIZED_H_
#define MEMORY_UNINITIALIZED_H_

//#include "traits.hpp"
#include "memory_construct.hpp"

namespace lix
{

template<class InputIterator, class ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first,
	InputIterator last,ForwardIterator result) {
	auto itr = result;
	for (; first != last; ++first, ++itr) construct(&*itr, *first);
	return itr;
}

//template<class ForwardIterator, class size, class T1, class T2>
//ForwardIterator _uninitialized_fill_n(ForwardIterator first,
//	size n, const T1&x, T2*) {
//	ForwardIterator itr = first;
//	for(;n>0;--n,++cur) {
//		construct(&*itr,)
//	}
//}

template<class ForwardIterator,class Size,class T>
ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& x) {
	ForwardIterator itr = first;
	for (; n > 0; --n, ++itr) {
		construct(&*itr, x);
	}
	return itr;
}

template<class ForwardIterator, class T>
void uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& x) {
	ForwardIterator cur = first;
	for (; cur != last; ++cur) construct(cur, x);
}



}









#endif
