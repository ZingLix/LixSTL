#ifndef LIX_POINTER_TRAITS_H_
#define LIX_POINTER_TRAITS_H_

#include <cstddef>
#include "memory_utility.hpp"

namespace lix
{

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
