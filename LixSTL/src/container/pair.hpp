#ifndef PAIR_H_
#define PAIR_H_
#include <type_traits>

namespace lix
{
	template<class T1, class T2> 
	struct pair
	{
		using first_type = T1;
		using secont_type = T2;

		first_type first;
		secont_type second;

		pair():first(T1()),second(T2()){}
		pair(const T1& a,const T2& b):first(a),second(b){}
	};

	template<class _Ty>
	using _pair_wrapper = typename std::decay_t<_Ty>::type;

	template< class T1, class T2 >
	constexpr pair<_pair_wrapper<T1>, _pair_wrapper<T2>> make_pair(T1&& t, T2&& u) {
		return pair<T1,T2>(std::forward<T1>(t), std::forward<T2>(u));
	}
}

#endif
