#ifndef QUEUE_H_
#define QUEUE_H_
#include "deque.hpp"

namespace lix
{
template<class T, class Container = deque<T>>
class queue
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
	explicit queue(const Container& cont) :c(cont) {}
	explicit queue(Container&& cont = Container()) :c(cont) {}
	queue(const queue& other) :c(other.c) {}
	queue(queue&& other) noexcept :c(std::move(other.c)) {}

	~queue(){}

	queue& operator=(const queue& other) {
		c = other;
		return *this;
	}
	queue& operator=(queue&& other) noexcept {
		c = std::move(other);
		return *this;
	}

	reference front() { return c.front(); }
	reference back() { return c.back(); }

	const_reference front() const { return c.front(); }
	const_reference back() const { return c.back(); }

	bool empty() { return c.empty(); }
	size_type size() { return c.size(); }

	void push(const value_type& value) { c.push_back(value); }
	void push(value_type&& value) { c.push_back(value); }
	void pop() { c.pop_front(); }
};

}

#endif
