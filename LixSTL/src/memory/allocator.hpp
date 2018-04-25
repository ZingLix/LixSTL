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
		void *address = ::operator new(n*sizeof(T));
		//TODO 内存分配失败扔出异常
		//if (address == nullptr) throw;
		return static_cast<T*>(address);
	}


	static void deallocate(T *p, size_t=0) {
		::operator delete(static_cast<void*>(&*p));
	}
};


}

#endif
