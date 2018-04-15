#ifndef LIST_H_
#define LIST_H_

#include "../include/memory"
#include "traits.hpp"

namespace lix
{

template<class T>
struct _list_node
{
	typedef _list_node* ptr;
	ptr prev;
	ptr next;
	T data;
};

template<class T,class Ref,class Ptr>
struct _list_iterator
{
	typedef _list_iterator<T, T&, T*> iterator;
	typedef _list_iterator<T, Ref, Ptr> self;

	typedef bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ptr ptr;
	typedef Ref ref;
	typedef _list_node<T>* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	link_type node;

	_list_iterator(link_type x):node(x){}
	_list_iterator():node(nullptr){}
	_list_iterator(const iterator& x) :node(x.node){}

	bool operator==(const self& x) const { return node == x.node; }
	bool operator!=(const self& x) const { return node != x.node; }
	ref operator*()const { return node->data; }
	ptr operator->() const { return &(operator*()); }
	self& operator++() {
		node = node->next;
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		operator++();
		return tmp;
	}
	self& operator--() {
		node = node->prev;
		return *this;
	}
	self operator--(int) {
		self tmp = *this;
		operator--();
		return tmp;
	}
};




template<class T,class Alloc=_alloc_template>
class list
{
protected:
	typedef _list_node<T> list_node;
	typedef simple_alloc<list_node, Alloc> allocator;
public:
	typedef list_node* link_type;
	typedef _list_iterator<T,T&,T*> iterator;

	typedef bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef _list_node<T>* ptr;
	typedef _list_node<T>& ref;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	link_type node;

	iterator begin() { return node->next; }
	iterator end() { return node; }
	bool empty() { return node->next == node; }
	size_type size() const {
		return distance(begin(), end());
	}
	ref front() { return *begin(); }
	ref back() { return *(--end()); }
	
protected:
	link_type get_node() { return allocator::allocate(); }
	void put_node(link_type p) { allocator::deallocate(p); }
	link_type create_node(const T& x) {
		link_type p = get_node();
		construct(p, x);
		return p;
	}
	void destroy_node(link_type p) {
		destroy(&p->data);
		put_node(p);
	}
	void init() {
		node = get_node();
		node->next = node;
		node->prev = node;
		//node->data = nullptr;
	}
	void transfer(iterator pos, iterator first, iterator last) {
		if (pos != last) {
			last.node->prev->next = pos.node;
			first.node->prev->next = last.node;
			pos.node->prev->next = first.node;
			auto tmp = pos.node->prev;
			pos.node->prev = last.node->prev;
			last.node->prev = first.node->prev;
			first.node->prev = tmp;
		}
	}
public:
	list() { init(); }

	iterator insert(iterator pos,const T& x) {
		link_type tmp = create_node(x);
		tmp->next = pos.node;
		tmp->prev = pos.node->prev;
		pos.node->prev->next = tmp;
		pos.node->prev = tmp;
		return tmp;
	}

	iterator erase(iterator pos) {
		link_type prev = pos.node->prev;
		link_type next = pos.node->next;
		prev->next = next;
		next->prev = prev;
		destroy_node(pos);
		return next;
	}

	void push_back(const T& x) { insert(end(), x); }
	void push_front(const T& x) { insert(begin(), x); }
	void pop_back() { erase(--end()); }
	void pop_front() { erase(begin()); }

	void clear() {
		link_type itr = node->next;
		while(itr!=node) {
			link_type tmp = itr;
			++itr;
			destroy_node(tmp);
		}
		node->next = node;
		node->prev = node;
	}

	void remove(const T& val) {
		iterator first = begin();
		iterator last = end();
		while(first!=last) {
			iterator next = first;
			++next;
			if (*first == val) erase(first);
			first = next;
		}
	}

	void unique() {
		iterator first = begin();
		iterator last = end();
		if(first==last) return;
		iterator next = first;
		while(++next!=last) {
			if (*first == *next) erase(next);
			else { first = next; }
			next = first;
		}
	}

	void splice(iterator pos,list& l) {
		if (!l.empty()) transfer(pos, l.begin(), l.end());
	}

	void splice(iterator pos,iterator itr,list&) {
		iterator i = itr;
		++i;
		if(pos==i||pos==itr) return;
		transfer(pos, itr, i);
	}

	void splice(iterator pos,iterator first,iterator last) {
		if (first != last) transfer(pos, first, last);
	}

	//TODO
	//void merge(list& x);
	//void reverse();
	//void sort();

};



}



#endif
