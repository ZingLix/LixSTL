#ifndef LIX_POINTER_TRAITS_H_
#define LIX_POINTER_TRAITS_H_

#include <cstddef>
#include "memory_utility.hpp"

namespace lix
{

	template<class T>
	struct _first_parameter
	{
		using type = T;
	};

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



	template<class Ptr>
	struct pointer_traits
	{
		using element_type = typename _element_type<Ptr>::type;
		using pointer = Ptr;
		using difference_type = typename _ptr_difference_type<Ptr>::type;

		template<class _Other>
		using rebind = typename _rebind_alias<Ptr, _Other>::type;

		static pointer pointer_to(Ptr& _Val)
		{
			return (Ptr::pointer_to(_Val));
		}
	};

	template<class T>
	struct pointer_traits<T*>
	{
		typedef T* pointer;
		typedef T element_type;
		typedef ptrdiff_t difference_type;

		template<class Other>
		using rebind = Other * ;

		static pointer pointer_to(element_type& r) {
			return addressof(r);
		}
	};



}


#endif
