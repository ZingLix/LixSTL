#ifndef LIST_H_
#define LIST_H_

#include "../include/memory"
#include "../traits/iterator_traits.hpp"
#include <list>
namespace lix
{

template<class T>
struct _list_node
{
	_list_node(T x):prev(nullptr),next(nullptr),data(x) {
	}

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
	typedef Ptr pointer;
	typedef Ref reference;
	typedef _list_node<T>* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	link_type node;

	_list_iterator(link_type x):node(x){}
	_list_iterator():node(nullptr){}
	_list_iterator(const iterator& x) :node(x.node){}

	bool operator==(const self& x) const { return node == x.node; }
	bool operator!=(const self& x) const { return node != x.node; }
	reference operator*()const { return node->data; }
	pointer operator->() const { return &(operator*()); }
	self& operator++() {
		node = node->next;
		return *this;
	}
	self operator++(int)  {
		self tmp = *this;
		operator++();
		return tmp;
	}
	self& operator--() {
		node = node->prev;
		return *this;
	}
	self operator--(int)  {
		self tmp = *this;
		operator--();
		return tmp;
	}
};




template<class T,class Allocator=allocator<T>>
class list
{
protected:
	using list_node = _list_node<T>;
	using allocator = typename  allocator_traits<Allocator>::template rebind_alloc<_list_node<T>>;
	using link_type = list_node * ;

public:
	using value_type = T;
	using allocator_type = Allocator;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using pointer = typename allocator_traits<Allocator>::pointer;
	using const_pointer = typename allocator_traits<Allocator>::const_pointer;
	using iterator = _list_iterator<T, reference, pointer>;
	using const_iterator = const iterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
	allocator allocator_;
	link_type node;

public:
	allocator_type get_allocator() const {
		return allocator_;
	}
	iterator begin() const { return node->next; }
	iterator end() const { return node; }
	bool empty() { return node->next == node; }
	size_type size() {
		return distance(begin(), end());
	}
	reference front() { return *begin(); }
	reference back() { return *(--end()); }
	
protected:
	link_type get_node() { return allocator::allocate(1); }
	void put_node(link_type p) { allocator::deallocate(p); }
	link_type create_node(const T& x) {
		link_type p = get_node();
		allocator_traits<allocator>::construct(allocator_, p, x);
		return p;
	}
	void destroy_node(link_type p) {
	//	allocator_traits<allocator>::destroy(allocator_, p);
		put_node(p);
	}
	void init(Allocator alloc=Allocator()) {
		allocator_ = alloc;
		node = get_node();
		node->next = node;
		node->prev = node;
	}
	void init_with_initial(size_type count, const T& value, const Allocator& alloc = Allocator()) {
		init(alloc);
		while (count--) {
			push_back(value);
		}
	}
	template< class InputIt >
	void init_with_iterator(InputIt first, InputIt last, const Allocator& alloc = Allocator()) {
		init(alloc);
		iterator itr = first;
		for (; itr != last; ++itr) {
			push_back(*itr);
		}
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
	list(size_type count, const T& value=T(), const Allocator& alloc = Allocator()) {
		init_with_initial(count, value, alloc);
	}
	// TODO DefaultInsertable?
	//explicit list(size_type count, const Allocator& alloc = Allocator()) {
	//	init_with_initial(count, T(), alloc);
	//}

	// TODO InputIt satisfies InputIterator
	//template< class InputIt >
	//list(InputIt first, InputIt last, const Allocator& alloc = Allocator()) {
	//	init_with_iterator(first, last, alloc);
	//}
	list(const list& other, const Allocator& alloc=Allocator()) {
		init_with_iterator(other.begin(), other.end(), alloc);
		/*init(alloc);
		iterator first = other.begin();
		for (; first != other.end(); ++first) {
			push_back(*first);
		}*/
	}
	list(list&& other) noexcept {
		allocator_ = std::move( other.allocator_);
		node = other.node;
		other.node = nullptr;
	}
	list(list&& other, const Allocator& alloc) {
		if(alloc==other.get_allocator()) {
			allocator_ = alloc;
			node = other.node;
			other.node = nullptr;
		}else {
			//TODO
		}
	}
	list(std::initializer_list<T> initlist,
		const Allocator& alloc = Allocator()) {
		init(alloc);
		for (auto it = initlist.begin(); it != initlist.end(); ++it) push_back(*it);
	}
	~list() {
		_clear();
	}

	list& operator=(const list& other) {
		_clear();
		init_with_iterator(other.begin(), other.end(), other.get_allocator());
		return *this;
	}
	list& operator=(list&& other) noexcept(allocator_traits<Allocator>::is_always_equal::value) {
		_clear();
		allocator_ = other.get_allocator();
		node = other.node;
		other.node = nullptr;
		return *this;
	}
	list& operator=(std::initializer_list<T> ilist) {
		clear();
		for (auto it = ilist.begin(); it != ilist.end(); ++it) push_back(*it);
		return *this;
	}

	iterator insert(const_iterator pos,const T& x) {
		link_type tmp = create_node(x);
		tmp->next = pos.node;
		tmp->prev = pos.node->prev;
		pos.node->prev->next = tmp;
		pos.node->prev = tmp;
		return tmp;
	}
	iterator insert(const_iterator pos, size_type count, const T& value) {
		iterator tmp = pos;
		--tmp;
		while(count--) {
			insert(pos, value);
		}
		++tmp;
		return tmp;
	}
	//TODO InputIt satisfies InputIterator
	//template< class InputIt >
	//iterator insert(const_iterator pos, InputIt first, InputIt last);
	iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
		auto it = ilist.begin();
		auto tmp = insert(pos, *it);
		for (++it; it != ilist.end(); ++it) insert(pos, *it);
		return tmp;
	}

	template< class... Args >
	iterator emplace(const_iterator pos, Args&&... args) {
		link_type p = get_node();
		link_type t = pos.node;
		allocator_traits<allocator>::construct(allocator_, p, args...);

		p->next = t;
		t->prev->next = p;
		p->prev = t->prev;
		t->prev = p;

		auto tmp = pos;
		return --tmp;
	}

	template< class... Args >
	reference emplace_back(Args&&... args) {
		iterator it = emplace(end(), args...);
		return it.node->data;
	}
	template< class... Args >
	reference emplace_front(Args&&... args) {
		iterator it = emplace(begin(), args...);
		return it.node->data;
	}
	iterator erase(iterator pos) {
		link_type prev = pos.node->prev;
		link_type next = pos.node->next;
		prev->next = next;
		next->prev = prev;
		destroy_node(pos.node);
		return next;
	}
	iterator erase(iterator first,iterator last) {
		for(;first!=last;/*++first*/) {
			first= erase(first);
		}
		return last;
	}

	void push_back(const T& x) { insert(end(), x); }
	void push_front(const T& x) { insert(begin(), x); }
	void pop_back() { erase(--end()); }
	void pop_front() { erase(begin()); }

	void clear() noexcept {
		if (node != nullptr) {
			link_type itr = node->next;
			while (itr != node) {
				link_type tmp = itr;
				itr = itr->next;
				destroy_node(tmp);
			}
			node->next = node;
			node->prev = node;
		}
	}

	void _clear() {
		clear();
		destroy_node(node);
	}

	void swap(list& other) noexcept(allocator_traits<Allocator>::is_always_equal::value) {
		auto tmp_node = other.node;
		auto tmp_allocator = other.allocator_;
		other.node = node;
		other.allocator_ = allocator_;
		node = tmp_node;
		allocator_ = tmp_allocator;
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

	void splice(iterator pos,list&, iterator itr) {
		iterator i = itr;
		++i;
		if(pos==i||pos==itr) return;
		transfer(pos, itr, i);
	}

	void splice(iterator pos,iterator first,iterator last) {
		if (first != last) transfer(pos, first, last);
	}

	void merge(list& other) {
		if (get_allocator() != other.get_allocator()) return;
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = other.begin();
		iterator last2 = other.end();
		if(first1==first2&&last1==last2) return;
		while (first1!=last1 && first2!=last2) {
			if(*first2<*first1) {
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}else {
				++first1;
			}
		}
		if (first2 != last2) transfer(last1, first2, last2);
	}
	
	//TODO	
	//template <class Compare>
	//void merge(list&& other, Compare comp);
	
	void reverse() noexcept {
		if (node->next == node || node->next->next == node) return;
		iterator itr = begin();
		++itr;
		while(itr!=end()) {
			iterator old = itr;
			++itr;
			transfer(begin(), old, itr);
		}
	}
	
	void sort() {
		if (node->next == node || node->next->next == node) return;
		list<T, Allocator> carry;
		list<T, Allocator> counter[64];
		int fill = 0;
		while(!empty()) {
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while(i<fill&&!counter[i].empty()) {
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill)++fill;
		}
		for (int i = 1; i < fill; ++i) 
			counter[i].merge(counter[i - 1]);
		swap(counter[fill - 1]);
	}

};



}



#endif
