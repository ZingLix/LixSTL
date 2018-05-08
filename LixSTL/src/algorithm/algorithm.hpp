#ifndef ALGORITHM_H_
#define ALGORITHM_H_

namespace lix
{
template<class T>
T& max(T& lfs,T& rfs) {
	return lfs < rfs ? rfs : lfs;
}

template<class ForwardIterator>
void copy(ForwardIterator first, ForwardIterator end, ForwardIterator new_pos) {
	while (first < end) {
		*new_pos = *first;
		++first;
		++new_pos;
	}
}

}

#endif
