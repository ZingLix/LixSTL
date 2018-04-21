#ifndef ALLOCATOR_TRAITS_H_
#define ALLOCATOR_TRAITS_H_
#include "allocator.hpp"
#include "pointer_traits.hpp"
#include "memory_utility.hpp"
#include <type_traits>
#include <limits>


namespace lix
{
	
	template< class T >
	T* addressof(T& arg)
	{
		return reinterpret_cast<T*>(
			&const_cast<char&>(
				reinterpret_cast<const volatile char&>(arg)));
	}

	template<class T, class Alloc, class = void>
	struct _pointer
	{
		using type = T * ;
	};

	template<class T, class Alloc>
	struct _pointer<T, Alloc, void_t<typename Alloc::pointer>>
	{
		using type = typename Alloc::pointer;
	};

	template<class T, class Alloc, class = void>
	struct _const_pointer
	{
		using type = typename pointer_traits<T>::template rebind<const T>;
	};

	template<class T, class Alloc>
	struct _const_pointer<T, Alloc, void_t<typename Alloc::const_pointer>>
	{
		using type = typename Alloc::const_pointer;
	};

	template<class T, class Alloc, class = void>
	struct _void_pointer
	{
		using type = typename pointer_traits<T>::template rebind<void>;
	};

	template<class T, class Alloc>
	struct _void_pointer<T, Alloc, void_t<typename Alloc::void_pointer>>
	{
		using type = typename Alloc::void_pointer;
	};

	template<class T, class Alloc, class = void>
	struct _const_void_pointer
	{
		using type = typename pointer_traits<T>::template rebind<const void>;
	};

	template<class T, class Alloc>
	struct _const_void_pointer<T, Alloc, void_t<typename Alloc::const_void_pointer>>
	{
		using type = typename Alloc::const_void_pointer;
	};

	template<class T, class Alloc, class = void>
	struct _difference_type
	{
		using type = typename pointer_traits<T>::difference_type;
	};

	template<class T, class Alloc>
	struct _difference_type<T, Alloc, void_t<typename Alloc::difference_type>>
	{
		using type = typename Alloc::difference_type;
	};

	template<class T, class Alloc, class = void>
	struct _size_type
	{
		using type = std::make_unsigned<T>;
	};

	template<class T, class Alloc>
	struct _size_type<T, Alloc, void_t<typename Alloc::size_type>>
	{
		using type = typename Alloc::size_type;
	};

	template<class T, class Alloc, class = void>
	struct _rebind_alloc
	{
		using type = _replace_first_parameter<T,Alloc>;
	};

	template<class T, class Alloc>
	struct _rebind_alloc<T, Alloc, void_t<typename Alloc::template rebind<T>::other>>
	{
		using type = typename Alloc::template rebind<T>::other;
	};

	//template<typename T>
	//struct has_construct
	//{
	//	template<typename U, size_t(U::*)() const> struct SFINAE {};
	//	template<typename U> static char Test(SFINAE<U, &U::used_memory>*);
	//	template<typename U> static int Test(...);
	//	static const bool Has = sizeof(Test<T>(0)) == sizeof(char);
	//};

	template<typename T>
	struct has_construct
	{
		template <typename C> static char test(decltype(&C::construct)); 
		template <typename C> static int test(...);
		const static bool value = sizeof(test<T>(0)) == sizeof(char);
	};

	template<typename T>
	struct has_destroy
	{
		template <typename C> static char test(decltype(&C::destroy)); 
		template <typename C> static int test(...);
		const static bool value = sizeof(test<T>(0)) == sizeof(char);
	};

	template<typename T>
	struct has_max_size
	{
		template <typename C> static char test(decltype(&C::max_size));
		template <typename C> static int test(...);
		const static bool value = sizeof(test<T>(0)) == sizeof(char);
	};

	template<class Alloc>
	struct allocator_traits
	{
		typedef Alloc allocator_type;
		typedef typename Alloc::value_type value_type;
		using pointer = typename _pointer<value_type,Alloc>::type;
		using const_pointer = typename _const_pointer<value_type,Alloc>::type;
		using void_pointer = typename _void_pointer<value_type, Alloc>::type;
		using const_void_pointer = typename _const_void_pointer<value_type, Alloc>::type;
		using difference_type = typename _difference_type<value_type, Alloc>::type;
		using size_type = typename _size_type<difference_type, Alloc>::type;

		template<class U>
		using rebind_alloc = typename _rebind_alloc<U, Alloc>::type;
		template<class U>
		using rebind_traits = allocator_traits<rebind_alloc<U>>;

		static pointer allocate(Alloc& a,size_type n) {
			return a.allocate(n);
		}
		static void deallocate(Alloc& a,pointer p,size_type n) {
			a.deallocate(p, n);
		}
		template< class T, class... Args >
		static void construct(Alloc& a, T* p, Args&&... args) {
			if(has_construct<Alloc>::value) {
				a.construct(p, std::forward<Args>(args)...);
			}
			else {
				::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
			}
		}
		template< class T >
		static void destroy(Alloc& a, T* p) {
			if(has_destroy<Alloc>::value) {
				a.destroy(p);
			}else {
				p->~destroy();
			}
		}
		static size_type max_size(const Alloc& a) {
			if(has_max_size<Alloc>::value) {
				return a.max_size();
			}else {
				return std::numeric_limits<size_type>::max() / sizeof(value_type);
			}
		}
	};



}



#endif
