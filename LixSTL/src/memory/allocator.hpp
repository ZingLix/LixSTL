#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_
#include <new>

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

	typedef T value_type;

	static T* allocate(size_t n) {
		void *address = ::operator new(n);
		//TODO 内存分配失败扔出异常
		//if (address == nullptr) 
		return static_cast<T*>(address);
	}


	static void deallocate(T *p, size_t) {
		::operator delete(p);
	}
};


}

#endif
