#ifndef ALGORITHM_H_
#define ALGORITHM_H_
#include <functional>

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

template<class T>
struct identity
{
	template< class T1>
	constexpr T1&& operator()(T1&& t) const noexcept {
		return std::forward<T1>(t);
	}
};

template<class T1,class T2>
struct select1st
{
	constexpr T1 operator()(std::pair<T1, T2> t) const noexcept {
		return t.first;
	}
};


}

#endif
