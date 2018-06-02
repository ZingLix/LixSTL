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

	template<class T>
	struct _rb_tree_node
	{
		using color_type = _rb_tree_color;
		using link_type = _rb_tree_node<T>*;

		_rb_tree_node(const T& val):parent(nullptr),left(nullptr),right(nullptr),color(_rb_tree_red),value(val){}

		link_type parent;
		link_type left;
		link_type right;

		color_type color;

		T value;
	};

	template<class T,class Ref,class Ptr>
	struct _rb_tree_iterator
	{
		using iterator_category = bidirectional_iterator_tag;
		using difference_type = ptrdiff_t;

		using value_type = T;
		using reference = Ref;
		using pointer = Ptr;
		using iterator = _rb_tree_iterator<T, T&, T*>;
		using const_iterator = _rb_tree_iterator<T, const T&, const T*>;
		using self = _rb_tree_iterator<T, Ref, Ptr>;
		using link_type = _rb_tree_node<T>*;

		link_type node;

		_rb_tree_iterator():node(nullptr) {}
		_rb_tree_iterator(link_type& x) { node = x; }
		_rb_tree_iterator(const iterator& it) { node = it.node; }

		void increment() {
			if (node->right != nullptr) {
				node = node->right;
				while (node->left != nullptr) node = node->left;
			}
			else {
				link_type p = node->parent;
				while (node == p->right) {
					node = p;
					p = p->parent;
				}
				if (node->right != p) node = p;
			}
		}
		void decrement() {
			if (node->color == _rb_tree_red && node->parent->parent == node) {
				node = node->right;
			}
			else if (node->left != nullptr) {
				node = node->left;
				while (node->right != nullptr) node = node->right;
			}
			else {
				link_type p = node->parent;
				while (node == p->left) {
					node = p;
					p = p->parent;
				}
				node = p;
			}
		}

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
		using rb_tree_node = _rb_tree_node<Value>;
		using node_allocator =typename allocator_traits<Alloc>::template rebind_alloc<_rb_tree_node<Value>>;
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
		link_type get_node() { return node_allocator::allocate(1); }
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

		static link_type left(link_type x) { return x->left; }
		static link_type right(link_type x) { return x->right; }
		static link_type parent(link_type x) { return x->parent; }
		static reference value(link_type x) { return x->value; }
		static link_type& key(link_type x) { return KeyOfValue()(x->left); }
		static color_type& color(link_type x) { return x->color; }

		static link_type min(link_type x) {
			while (x->left != nullptr) {
				x = x->left;
			}
			return x;
		}
		static link_type max(link_type x) {
			while (x->right != nullptr) {
				x = x->right;
			}
			return x;
		}

	public:
		using iterator = _rb_tree_iterator<value_type, reference, pointer>;
		using const_iterator = const iterator;

	private:
		const color_type RED = _rb_tree_red;
		const color_type BLACK = _rb_tree_black;
		void init() {
			header = get_node();
			header->color = RED;
			root() = nullptr;
			leftmost() = header;
			rightmost() = header;
		}

	public:
		rb_tree(const Compare& compare=Compare()):alloc_(),node_count(0),comp(compare) {
			init();
		}
		~rb_tree() {
			clear(root());
			put_node(header);
		}
		Compare key_comp() const { return comp; }
		iterator begin() { return leftmost(); }
		iterator end() { return header; }
		bool empty() const { return node_count == 0; }
		size_type size() const { return node_count; }

	protected:
		void left_rotate(link_type x) {
			link_type y = x->right;
			x->right = y->left;
			if(y->left!=nullptr)
				y->left->parent = x;
			y->parent = x->parent;
			if(root()==x) {
				root() = y;
			}else if (x->parent->left == x) x->parent->left = y;
			else { x->parent->right = y; }
			y->left = x;
			x->parent = y;
		}

		void right_rotate(link_type y) {
			link_type x = y->left;
			y->left = x->right;
			if (x->right != nullptr)
				x->right->parent = y;
			x->parent = y->parent;
			if (root() == y) root() = x;
			else if (y->parent->left == y) y->parent->left = x;
			else y->parent->right = x;
			x->right = y;
			y->parent = x;
		}

		void clear(link_type it) {
			auto parent=it;
			if ( it->left!=nullptr ) {
				clear(left(it));
			}
			if(it->right!=nullptr) {
				clear(right(it));
			}
			destory_node(it);
		}
	public:
		iterator insert(value_type x) {
			auto node = create_node(x);
			link_type it = root();
			if(it==nullptr) {
				header->parent = node;
				node->parent = header;
				leftmost() = node;
				rightmost() = node;
				node->left = node->right = nullptr;
				node->color = BLACK;
			} else {
				link_type tmp = parent(it);
				while (it != nullptr) {
					tmp = it;
					if (comp(x, value(it)))
						it = left(it);
					else { it = right(it); }
				}
				node->parent = tmp;
				if (comp(x, value(tmp))) {
					tmp->left = node;
					if (tmp == header) {
						root() = node;
						rightmost() = node;
					}
					else if (tmp == leftmost()) {
						leftmost() = node;
					}
				}
				else {
					tmp->right = node;
					if (tmp == rightmost())
						rightmost() = node;
				}
				node->left = node->right = nullptr;
				node->color = RED;
				insert_aux(node);
			}
			++node_count;
			return iterator(node);
		}
	protected:
		void insert_aux(link_type& p) {
			while (p!=root()&& p->parent->color==RED ) {
				if(p->parent==p->parent->parent->left) {
					auto y = p->parent->parent->right;
					if(y!=nullptr&& y->color==RED) {
						p->parent->color = BLACK;
						y->color = BLACK;
						p->parent->parent->color = RED;
						p = parent(parent(p));
					} else {
						if (p == p->parent->right) {
							p = parent(p);
							left_rotate(p);
						}
						p->parent->color = BLACK;
						p->parent->parent->color = RED;
						right_rotate(parent(parent(p)));
					}
				}else {
					auto y = p->parent->parent->left;
					if (y != nullptr&&y->color == RED) {
						p->parent->color = BLACK;
						y->color = BLACK;
						p->parent->parent->color = RED;
						p = parent(parent(p));
					}
					else {
						if (p->parent!=header&& p == p->parent->left) {
							p = parent(p);
							right_rotate(p);
						}
						p->parent->color = BLACK;
						p->parent->parent->color = RED;
						left_rotate(parent(parent(p)));
					}
				}
			}
			color(root()) = BLACK;
		}
	public:
		void remove(value_type val) {
			link_type it = root();
			while ( it!= nullptr ) {
				if (comp(val, it->value)) {
					it = it->left;
				}
				else if (val == it->value) {
					_remove(it);
					return;
				}
				else {
					it = it->right;
				}
			}
			
			
		}

		void _transplant(link_type u,link_type v) {
			if(u->parent==header) {
				root() = v;
			}else if(u==u->parent->left) {
				u->parent->left = v;
			} else {
				u->parent->right = v;
			}
			if(v!=nullptr)
				v->parent = u->parent;
		}

		void _remove(link_type p) {
			link_type y = p;
			bool ori_color = y->color;
			link_type x;
			bool is_left = (p == p->parent->left) ? true : false;
			if(p->left==nullptr) {
				x = p->right;
				_transplant(p, p->right);
			}else if(p->right==nullptr) {
				x = p->left;
				_transplant(p, p->left);
			} else {
				y = min(p->right);
				ori_color = y->color;
				x = y->right;
				if(y->parent!=p) {
					_transplant(y, y->right);
					y->right = p->right;
					y->right->parent = y;
				}else {
					x->parent = y;
				}
				_transplant(p, y);
				y->left = p->left;
				y->left->parent = y;
				y->color = p->color;
			}
			if (ori_color == BLACK) remove_aux(p, is_left);
			--node_count;






			// node to be deleted
			//link_type y = (left(p) == nullptr || right(p) == nullptr) ? p : successor(p);
			//// y's child
			//link_type y_child = (y->left != nullptr) ? y->left : y->right;
			//if(y_child!=nullptr)
			//	y_child->parent = y->parent;
			//bool is_left;
			//if(y==y->parent->left) {
			//	y->parent->left = y_child;
			//	is_left = true;
			//}else {
			//	y->parent->right = y_child;
			//	is_left = false;
			//}
			//if(y!=p) value(p)=value(y);
			//if (y->color == BLACK) remove_aux(y_child,is_left);
			destory_node(y);


		}
	protected:
		link_type successor(link_type& p) {
			if (p->right != nullptr)
				return min(p->right);
			auto y = p->parent;
			while ( y!=nullptr && p==y->right ) {
				p = y;
				y = y->parent;
			}
			return y;
		}

		link_type predecessor(link_type& p) {
			if (p->left != nullptr)
				return max(p->left);
			auto y = p->parent;
			while (y != nullptr && p == y->left) {
				p = y;
				y = y->parent;
			}
			return y;
		}

		void remove_aux(link_type& x,bool is_left) {
			while ( x!=root()&&x->color==BLACK ) {
				if(is_left|| x==x->parent->left) {
					auto w = x->parent->right;
					if(w!=nullptr) {
						if (w->color == RED) {
							w->color = BLACK;
							x->parent->color = RED;
							left_rotate(x->parent);
							w = x->parent->right;
						}
						if ((w->left==nullptr|| w->left->color == BLACK )&&(w->right==nullptr || w->right->color == BLACK)) {
							w->color = RED;
							x = x->parent;
						}
						else if (w->right == nullptr||w->right->color == BLACK) {
							w->left->color = BLACK;
							w->color = RED;
							right_rotate(w);
							w = x->parent->right;
						}
						w->color = x->parent->color;
						x->parent->color = BLACK;
						if(w->right!=nullptr)
							w->right->color = BLACK;
						left_rotate(x->parent);
						x = root();
					}else {
						//if(x->parent->color==RED) {
						//	x->parent->color = BLACK;
						//	x = root();
						//}else {
						//	x = x->parent;
						//}
						//x->parent->color = BLACK;
						
					}
					
				}else {
					auto w = x->parent->left;
					if (w != nullptr) {
						if (w->color == RED) {
							w->color = BLACK;
							x->parent->color = RED;
							right_rotate(x->parent);
							w = x->parent->left;
						}
						if (w->left != nullptr&&w->right != nullptr&&w->right->color == BLACK && w->left->color == BLACK) {
							w->color = RED;
							x = x->parent;
						}
						else if (w->right != nullptr&&w->left->color == BLACK) {
							w->right->color = BLACK;
							w->color = RED;
							left_rotate(w);
							w = x->parent->left;
						}
						w->color = x->parent->color;
						x->parent->color = BLACK;
						if (w->right != nullptr)
							w->left->color = BLACK;
						right_rotate(x->parent);
						x = root();
					}
					
				}
				is_left = false;
			}
			x->color = BLACK;
		}
	};

	template <typename T>
	struct identity
	{
		T operator()(T x) const { return x; }
	};
}


#endif
