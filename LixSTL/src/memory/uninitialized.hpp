#ifndef MEMORY_UNINITIALIZED_H_
#define MEMORY_UNINITIALIZED_H_

//#include <iterator>
#include "memory_utility.hpp"
#include "../traits/iterator_traits.hpp"

namespace lix
{
	template<class InputIt, class ForwardIt >
	ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first) {
		using value_type= typename iterator_traits<ForwardIt>::value_type;
		ForwardIt current = d_first;
		try {
			for (; first != last; ++first, (void) ++current) {
				::new (static_cast<void*>(lix::addressof(*current))) value_type(*first);
			}
			return current;
		}
		catch (...) {
			for (; d_first != current; ++d_first) {
				d_first->~value_type();
			}
			throw;
		}
	}

	template< class InputIt, class Size, class ForwardIt >
	ForwardIt uninitialized_copy_n(InputIt first, Size count, ForwardIt d_first) {
		using value_type = typename iterator_traits<ForwardIt>::value_type;
		ForwardIt current = d_first;
		try {
			for (; count > 0; ++first, (void) ++current, --count) {
				::new (static_cast<void*>(lix::addressof(*current))) value_type(*first);
			}
		}
		catch (...) {
			for (; d_first != current; ++d_first) {
				d_first->~value_type();
			}
			throw;
		}
		return current;
	}


	template<class ForwardIt, class T>
	void uninitialized_fill(ForwardIt first, ForwardIt last, const T& value)
	{
		using value_type = typename iterator_traits<ForwardIt>::value_type;
		ForwardIt current = first;
		try {
			for (; current != last; ++current) {
				::new (static_cast<void*>(lix::addressof(*current))) value_type(value);
			}
		}
		catch (...) {
			for (; first != current; ++first) {
				first->~value_type();
			}
			throw;
		}
	}


	template< class ForwardIt, class Size, class T >
	ForwardIt uninitialized_fill_n(ForwardIt first, Size count, const T& value)
	{
		using value_type = typename iterator_traits<ForwardIt>::value_type;
		ForwardIt current = first;
		try {
			for (; count > 0; ++current, (void) --count) {
				::new (static_cast<void*>(lix::addressof(*current))) value_type(value);
			}
			return current;
		}
		catch (...) {
			for (; first != current; ++first) {
				first->~value_type();
			}
			throw;
		}
	}

}


#endif
