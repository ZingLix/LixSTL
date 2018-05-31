#ifndef RBTREE_H_
#define RBTREE_H_
#include "../traits/iterator_traits.hpp"
#include "../memory/allocator.hpp"
#include "../memory/allocator_traits.hpp"

namespace lix
{
	using _rb_tree_color = bool;
	const _rb_tree_color _rb_tree_red = false;
	const _rb_tree_color _rb_tree_black = true;

	struct _rb_tree_node_base
	{
		using color_type = _rb_tree_color;
		using base_ptr = _rb_tree_node_base * ;

		base_ptr parent;
		base_ptr left;
		base_ptr right;

		color_type color;

		static base_ptr min(base_ptr x) {
			while (x->left != nullptr) {
				x = x->left;
			}
			return x;
		}

		static base_ptr max(base_ptr x) {
			while (x->right!=nullptr) {
				x = x->right;
			}
			return x;
		}
	};

	template<class T>
	struct _rb_tree_node:public _rb_tree_node_base
	{
		using link_type = _rb_tree_node<T>*;
		T value;
	};

	struct _rb_tree_iterator_base
	{
		using base_ptr = _rb_tree_node_base::base_ptr;
		using iterator_category = bidirectional_iterator_tag;
		using difference_type = ptrdiff_t;

		base_ptr node;

		void increment() {
			if(node->right!=nullptr) {
				node = node->right;
				while (node->left != nullptr) node = node->left;
			}else {
				base_ptr p = node->parent;
				while (node==p->right) {
					node = p;
					p = p->parent;
				}
				if (node->right != p) node = p;
			}
		}
		void decrement() {
			if(node->color==_rb_tree_red&&node->parent->parent==node) {
				node = node->right;
			}else if(node->left!=nullptr) {
				node = node->left;
				while (node->right != nullptr) node = node->right;
			} else {
				base_ptr p = node->parent;
				while (node==p->left) {
					node = p;
					p = p->parent;
				}
				node = p;
			}
		}
	};

	template<class T,class Ref,class Ptr>
	struct _rb_tree_iterator:public _rb_tree_iterator_base
	{
		using value_type = T;
		using reference = Ref;
		using pointer = Ptr;
		using iterator = _rb_tree_iterator<T, T&, T*>;
		using const_iterator = _rb_tree_iterator<T, const T&, const T*>;
		using self = _rb_tree_iterator<T, Ref, Ptr>;
		using link_type = _rb_tree_node<T>*;

		_rb_tree_iterator() {}
		_rb_tree_iterator(link_type x) { node = x; }
		_rb_tree_iterator(const iterator& it) { node = it.node; }

		using link_type = _rb_tree_node<T>*;
		T operator*() {
			return static_cast<link_type>(node)->value;
		}
		//T operator->();
		T operator++() {
			increment();
			return *this;
		}
		T operator++(int) {
			self tmp = *this;
			increment();
			return tmp;
		}
		T operator--() {
			decrement();
			return *this;
		}
		T operator--(int) {
			self tmp = *this;
			decrement();
			return tmp;
		}

	};

	template<class Key,class Value,class KeyOfValue,class Compare,class Alloc=allocator<Value>>
	class rb_tree
	{
	protected:
		using void_pointer = void*;
		using base_ptr = _rb_tree_node_base * ;
		using rb_tree_node = _rb_tree_node<Value>;
		using node_allocator = Alloc;
		using color_type = _rb_tree_color;

	public:
		using key_type = Key;
		using value_type = Value;
		using pointer = value_type * ;
		using const_pointer = const pointer;
		using reference = value_type & ;
		using const_reference = const reference;
		using link_type = rb_tree_node * ;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
	protected:
		link_type get_node() { return node_allocator::allocate(); }
		void put_node(link_type x) { node_allocator::deallocate(x); }
		link_type create_node(const value_type& x) {
			link_type node = get_node();
			allocator_traits<node_allocator>::construct(alloc_,node, x);
			return node;
		}
		link_type clone_node(link_type x) {
			link_type node = create_node(x->value);
			node->color = x->color;
			node->left = node->right = nullptr;
			return node;
		}
		void destory_node(link_type p) {
			allocator_traits<node_allocator>::destroy(alloc_, p);
			put_node(p);
		}

	protected:
		node_allocator alloc_;
		size_type node_count;
		link_type header;
		Compare comp;

		link_type& root() { return header->parent; }
		link_type& leftmost() { return header->left; }
		link_type& rightmost() { return header->right; }

		static link_type& left(link_type x) { return x->left; }
		static link_type& right(link_type x) { return x->right; }
		static link_type& parent(link_type x) { return x->parent; }
		static reference value(link_type x) { return x->value; }
		static link_type& key(link_type x) { return KeyOfValue()(x->left); }
		static color_type& color(link_type x) { return x->color; }

		static link_type min(link_type x) {
			return _rb_tree_node_base::min(x);
		}
		static link_type max(link_type x) {
			return _rb_tree_node_base::max(x);
		}
	public:
		using iterator = _rb_tree_iterator<value_type, reference, pointer>;

	private:
		const color_type RED = _rb_tree_red;
		const color_type BLACK = _rb_tree_black;
		void init() {
			header = get_node();
			header->color = RED;
			root() = nullptr;
			leftmost() = nullptr;
			rightmost() = nullptr;
		}

	public:
		rb_tree(const Compare& compare=Compare()):alloc_(),node_count(0),comp(compare) {
			init();
		}
		~rb_tree() {
			clear();
			put_node(header);
		}
	public:
		Compare key_comp() const { return comp; }
		iterator begin() { return leftmost(); }
		iterator end() { return header; }
		bool empty() const { return node_count == 0; }

	};
}


#endif
