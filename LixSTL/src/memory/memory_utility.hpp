#ifndef LIX_MEMORY_UTILITY_H_
#define LIX_MEMORY_UTILITY_H_

namespace lix
{
	template<class... _Types>
	using void_t = void;

	template<class NewFirst, class T>
	struct _replace_first_parameter
	{
		using type = NewFirst;
	};

	template<class NewFirst, template<class, class...>class T,
		class OldFirst, class ... Other>
		struct _replace_first_parameter<NewFirst, T<OldFirst, Other...>>
	{
		using type = T<NewFirst, Other...>;
	};

	template< class T >
	T* addressof(T& arg)
	{
		return reinterpret_cast<T*>(
			&const_cast<char&>(
				reinterpret_cast<const volatile char&>(arg)));
	}
}
#endif
