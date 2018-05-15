#ifndef STACK_H_
#define STACK_H_
#include "deque.hpp"

namespace lix
{
template<class T,class Container=deque<T>>
class stack
{
public:
	using container_type = Container;
	using value_type =typename Container::value_type;
	using size_type = typename Container::size_type;
	using reference = typename Container::reference;
	using const_reference = typename Container::const_reference;

protected:
	Container c;

public:
	explicit stack(const Container& cont):c(cont){}
	explicit stack(Container&& cont = Container()):c(cont){}
	stack(const stack& other):c(other.c){}
	stack(stack&& other) noexcept:c(std::move(other.c)){}

	~stack(){}

	stack& operator=(const stack& other) { 
		c = other;
		return *this;
	}
	stack& operator=(stack&& other) noexcept {
		c = std::move(other);
		return *this;
	}

	reference top() {
		return c.back();
	}
	bool empty()/*const*/ { return c.empty(); }
	size_type size() /*const*/ {
		return c.size();
	}
	void push(const value_type& value) { c.push_back(value); }
	void push(value_type&& value) { c.push_back(value); }
	void pop() { c.pop_back(); }

};

}

#endif
