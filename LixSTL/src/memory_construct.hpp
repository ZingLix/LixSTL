#ifndef MEMORY_CONSTRUCT_H_
#define MEMORY_CONSTRUCT_H_

#include <new>

namespace lix
{
	template<class T1,class T2>
	void construct(T1* p,const T2& value) {
		new (p) T1(value);
	}

	template<class T>
	void destroy(T* ptr) {
		ptr->~T();
	}



}

#endif
