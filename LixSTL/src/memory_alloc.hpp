#ifndef MEMORY_CONSTRUCT
#define MEMORY_CONSTRUCT

namespace lix_out
{



class _alloc_template
{
public:
	static void * allocate(size_t n) {
		void *address=::operator new(n);
		// if(address==0) out of memory
		return address;
	}
	static void deallocate(void *p,size_t) {
		::operator delete(p);
	}
	//static void *reallocate(void *p,size_t old_size,size_t new_size) {
	//	void *address = ::operator new(new_size);
	//	for(int i=0;i<old_size;i++) {
	//		*(address + i) = *(p + i);
	//	}
	//}
};

template<class T,class Alloc=_alloc_template>
class simple_alloc
{
public:
	static T *allocate(size_t n) {
		return 0 == n ? nullptr : static_cast<T*>(Alloc::allocate(n * sizeof(T)));
	}
	static T *allocate() {
		return static_cast<T*>(Alloc::allocate(sizeof(T)));
	}
	static void deallocate(T *p,size_t n) {
		if (n != 0) Alloc::deallocate(p, n * sizeof(T));
	}
	static void deallocate(T *p) {
		Alloc::deallocate(p, sizeof(T));
	}
};




}


#endif
