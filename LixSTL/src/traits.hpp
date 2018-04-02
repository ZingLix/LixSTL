#ifndef TRAITS_H_
#define TRAITS_H_

#include <cstddef>

namespace lix
{
	//TODO 五种迭代器标志&继承
	struct input_iterator_tag{};
	struct output_iterator_tag{};
	struct forward_iterator_tag : public output_iterator_tag{};
	struct bidirectional_iterator_tag:public forward_iterator_tag{};
	struct random_access_iterator_tag:public bidirectional_iterator_tag{};

	template <class category,class T,class Distance=ptrdiff_t,class pointer=T*,class reference=T&>
	struct iterator
	{
		typedef category iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef pointer ptr;
		typedef reference ref;
	};

	template <class Itr>
	struct iterator_traits
	{
		typedef typename Itr::iterator_category iterator_category;
		typedef typename Itr::value_type value_type;
		typedef typename Itr::difference_type difference_type;
		typedef typename Itr::ptr ptr;
		typedef typename Itr::ref ref;
	};

	// TODO T* & CONST T* 特化	
	template <class T>
	struct iterator_traits<T*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* ptr;
		typedef T& ref;
	};

	template<class T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef const T* ptr;
		typedef const T& ref;
	};

	//TODO 传入参数仅用于类型推导
	//获得迭代器类型 category
	template<class Itr>
	typename iterator_traits<Itr>::iterator_category iterator_category(const Itr&) {
		return typename iterator_traits<Itr>::iterator_category;
	}

	//获得 distance type
	template<class Itr>
	typename iterator_traits<Itr>::difference_type* distance_type(const Itr&) {
		return static_cast<typename iterator_traits<Itr>::difference_type*>(nullptr);
	}

	//获得 value type
	template<class Itr>
	typename iterator_traits<Itr>::value_type* value_type(const Itr&) {
		return static_cast<typename iterator_traits<Itr>::value_type*>(nullptr);
	}


}





#endif
