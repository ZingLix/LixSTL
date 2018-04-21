#ifndef LIX_MEMORY_UTILITY_H_
#define LIX_MEMORY_UTILITY_H_

namespace lix
{
	template<class... _Types>
	using void_t = void;

	template<class T>
	struct _first_parameter;

	template<template<class, class...> class T,
		class First, class... Other>
		struct _first_parameter<T<First, Other...>>
	{
		using type = First;
	};

	template<class T, class = void>
	struct _element_type
	{
		using type = typename _first_parameter<T>::type;
	};

	template<class T>
	struct _element_type<T, void_t<typename T::element_type>>
	{
		using type = typename T::element_type;
	};


	template<class T, class = void>
	struct _ptr_difference_type
	{
		using type = ptrdiff_t;
	};

	template<class T>
	struct _ptr_difference_type<T, void_t<typename T::difference_type>>
	{
		using type = typename T::difference_type;
	};

	template<class NewFirst, class T>
	struct _replace_first_parameter;

	template<class NewFirst, template<class, class...>class T,
		class OldFirst, class ... Other>
		struct _replace_first_parameter<NewFirst, T<OldFirst, Other...>>
	{
		using type = T<NewFirst, Other...>;
	};

	template<class T, class Other, class = void>
	struct _rebind_alias
	{
		using type = typename _replace_first_parameter<Other, T>::type;
	};

	template<class T, class Other>
	struct _rebind_alias<T, Other, void_t<typename T::template rebind<Other>>>
	{
		using type = typename T::template rebind<Other>;
	};

	


}
#endif
