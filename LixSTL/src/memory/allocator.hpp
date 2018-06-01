#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_
#include <new>
#include "../traits/iterator_traits.hpp"

namespace lix
{
	
template<class T>
struct allocator
{
	allocator() noexcept{}
	allocator(const allocator& other) noexcept{}
	template<class U>
	allocator(const allocator<U>& other) noexcept{}

	~allocator(){}

	using value_type = T;
	using propagate_on_container_move_assignment = true_type;
	using is_always_equal = true_type;

	static T* allocate(size_t n) {
		void *address = ::operator new(n*sizeof(T));
		//if (address == nullptr) throw;
		return static_cast<T*>(address);
	}


	static void deallocate(T *p, size_t=0) {
		::operator delete(static_cast<void*>(&*p));
	}
};

template< class T1, class T2 >
bool operator==(const allocator<T1>& lhs, const allocator<T2>& rhs) {
	return true;
}

template< class T1, class T2 >
bool operator!=(const allocator<T1>& lhs, const allocator<T2>& rhs) {
	return false;
}

}

#endif
