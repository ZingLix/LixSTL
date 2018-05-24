#ifndef AlGORITHM_HEAP_H_
#define ALGORITHM_HEAP_H_
#include "../traits/iterator_traits.hpp"

namespace lix
{
	template<class RandomIt, class Distance, class T>
	void _push_heap(RandomIt first, Distance holeIdx, Distance topIdx, T value) {
		Distance parent = (holeIdx - 1) / 2;
		while (holeIdx > topIdx && *(first + parent) < value) {
			*(first + holeIdx) = *(first + parent);
			holeIdx = parent;
			parent = (holeIdx - 1) / 2;
		}
		#pragma warning(disable: 4244)
		*(first + holeIdx) = value;
	}

	template<class RandomIt, class T, class Distance>
	void _push_heap_aux(RandomIt first, RandomIt last, T*, Distance*) {
		_push_heap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)));
	}

	template< class RandomIt >
	void push_heap(RandomIt first, RandomIt last) {
		_push_heap_aux(first, last, distance_type(first), value_type(first));
	}

	template<class RandomIt,class Distance,class T>
	void _adjust_heap(RandomIt first,Distance holeIdx,Distance len,T value) {
		Distance topIdx = holeIdx;
		Distance rightChild = 2 * holeIdx + 2;
		while(rightChild<len) {
			if (*(first + rightChild) < *(first + (rightChild - 1))) --rightChild;
			*(first + holeIdx) = *(first + rightChild);
			holeIdx = rightChild;
			rightChild = 2 * (rightChild + 1);
		}
		if(rightChild==len) {
			*(first + holeIdx) = *(first + rightChild - 1);
			holeIdx = rightChild - 1;
		}
		_push_heap(first, holeIdx, topIdx, value);
	}

	template<class RandomIt,class T,class Distance>
	void _pop_heap(RandomIt first,RandomIt last,RandomIt result,T value,Distance*) {
		*result = *first;
		_adjust_heap(first,Distance(0),Distance(last-first),value);
	}

	template<class RandomIt, class T>
	inline void _pop_heap_aux(RandomIt first, RandomIt last, T*) {
		_pop_heap(first, last - 1, last - 1, T(*(last -  1)), distance_type(first));
	}

	template< class RandomIt >
	void pop_heap(RandomIt first, RandomIt last) {
		_pop_heap_aux(first, last, value_type(first));
	}

	template<class RandomIt, class T, class Distance>
	void _make_heap(RandomIt first, RandomIt last, T*, Distance*) {
		Distance length = last - first;
		if (length < 2) return;
		Distance parent = (length - 2) / 2;
		while (true) {
			_adjust_heap(first, parent, length, T(*(first + last)));
			if(parent==0) return;
			--parent;
		}
	}

	template<class RandomIt>
	void make_heap(RandomIt first, RandomIt last) {
		_make_heap(first, last, value_type(first), distance_type(last));
	}



}
#endif
