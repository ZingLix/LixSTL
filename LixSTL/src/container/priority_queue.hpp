#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_
#include "vector.hpp"
#include "../algorithm/heap.hpp"

namespace lix
{

template<class T,class Container = lix::vector<T>,
	class Compare = std::less<typename Container::value_type>>
class priority_queue
{
public:
	using container_type = Container;
	using value_compare = Compare;
	using value_type = typename Container::value_type;
	using size_type = typename Container::size_type;
	using reference = typename Container::reference;
	using const_reference = typename Container::const_reference;

protected:
	Container c;
	Compare comp;

public:
	//priority_queue():c(){}
	priority_queue(const Compare& compare, const Container& cont):c(cont),comp(compare){}
	explicit priority_queue(const Compare& compare = Compare(),
		Container&& cont = Container()):c(cont),comp(compare){}
	priority_queue(const priority_queue& other):c(other){}
	priority_queue(priority_queue&& other) noexcept :c(other){}
	//TODO
	//template< class InputIt >
	//priority_queue(InputIt first, InputIt last,
	//	const Compare& compare, const Container& cont);
	//template< class InputIt >
	//priority_queue(InputIt first, InputIt last,
	//	const Compare& compare = Compare(),
	//	Container&& cont = Container());
	~priority_queue(){}

	priority_queue& operator=(const priority_queue& other) {
		c = other.c;
		comp = other.comp;
		return *this;
	}
	priority_queue& operator=(priority_queue&& other) noexcept{
		c = std::move(other.c);
		comp = std::move(other.comp);
		return *this;
	}

	const_reference top() /*const*/ { return c.front(); }
	bool empty() /*const*/ { return c.empty(); }
	size_type size() const { return c.size(); }

	void push(const value_type& value) {
		c.push_back(value);
		push_heap(c.begin(), c.end()/*, comp*/);
	}
	void pop() {
		pop_heap(c.begin(), c.end()/*,comp*/);
		c.pop_back();
	}
	template< class... Args >
	void emplace(Args&&... args) {
		c.emplace_back(std::forward<Args>(args)...);
		push_heap(c.begin(), c.end()/*, comp*/);
	}

};


}
#endif
