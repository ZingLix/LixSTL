#ifndef PAIR_H_
#define PAIR_H_

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

	//template< class T1, class T2 >
	//constexpr pair<V1, V2> make_pair(T1&& t, T2&& u) {
		
	//}
}

#endif
