#ifndef RBTREE_H_
#define RBTREE_H_
#include "../traits/iterator_traits.hpp"
#include "../memory/allocator.hpp"
#include "../memory/allocator_traits.hpp"
#include <queue>

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
		self operator++() {
			increment();
			return *this;
		}
		self operator++(int) {
			self tmp = *this;
			increment();
			return tmp;
		}
		self operator--() {
			decrement();
			return *this;
		}
		self operator--(int) {
			self tmp = *this;
			decrement();
			return tmp;
		}

	};

	template<class Value,class Compare,class Alloc=allocator<Value>>
	class rb_tree
	{
	protected:
		using void_pointer = void*;
		using rb_tree_node = _rb_tree_node<Value>;
		using node_allocator =typename allocator_traits<Alloc>::template rebind_alloc<_rb_tree_node<Value>>;
		using color_type = _rb_tree_color;
		static const color_type RED = _rb_tree_red;
		static const color_type BLACK = _rb_tree_black;

	public:
		//using key_type = Key;
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
		//static link_type& key(link_type x) { return KeyOfValue()(x->left); }
		static color_type& color(link_type x) { return x->color; }

		color_type colorOf(link_type x) { return x == nullptr ? BLACK : x->color; }

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
			if(it!=nullptr) {
				if (it->left != nullptr) {
					clear(left(it));
				}
				if (it->right != nullptr) {
					clear(right(it));
				}
				destory_node(it);
			}
		}
	public:
		iterator insert(value_type& x) {
			iterator p;
			link_type it = root();
			if(it==nullptr) {
				return create_root(x);
			} else {
				link_type tmp = parent(it);
				while (it != nullptr) {
					tmp = it;
					if (comp(x, value(it)))
						it = left(it);
					else { it = right(it); }
				}
				p= _insert(tmp, x);
			}
			++node_count;
			return p;
		}

		std::pair<iterator,bool> insert_unique(value_type& x) {
			iterator p;
			link_type it = root();
			++node_count;
			if (it == nullptr) {
				return std::make_pair(create_root(x), true);
			}
			else {
				link_type tmp = parent(it);
				while (it != nullptr) {
					tmp = it;
					if (comp(x, value(it)))
						it = left(it);
					else { it = right(it); }
				}
				link_type t = tmp;
				while(t!=header) {
					if (!(comp(t->value, x) || comp(x, t->value))) {
						--node_count;
						return std::make_pair(iterator(tmp), false);
					}
					t = t->parent;
				}

				
				p = _insert(tmp, x);
			}
			return std::make_pair(p, true);
		}

		iterator create_root(value_type& val) {
			auto node = create_node(val);
			header->parent = node;
			node->parent = header;
			leftmost() = node;
			rightmost() = node;
			node->left = node->right = nullptr;
			node->color = BLACK;
			return iterator(node);
		}

		iterator _insert(link_type x,value_type& val) {
			auto node = create_node(val);
			node->parent = x;
			if (!comp(value(x),val)) {
				x->left = node;
				if (x == header) {
					root() = node;
					rightmost() = node;
				}
				else if (x == leftmost()) {
					leftmost() = node;
				}
			}
			else {
				x->right = node;
				if (x == rightmost())
					rightmost() = node;
			}
			node->color = RED;
			insert_aux(node);
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

		void _transplant(link_type u, link_type v) {
			if (u->parent == header) {
				root() = v;
			}
			else if (u == u->parent->left) {
				u->parent->left = v;
			}
			else {
				u->parent->right = v;
			}
			if (v != nullptr)
				v->parent = u->parent;
		}

	public:
		link_type _find(value_type val) {
			link_type it = root();
			while (it != nullptr) {
				if (comp(val, it->value)) {
					it = it->left;
				}
				else if (val == it->value) {
					return it;
				}
				else {
					it = it->right;
				}
			}
			return it;
		}

		//iterator find(const Key& k) {
		//	link_type y = header;
		//	link_type x = root();

		//	while(x!=nullptr) {
		//		if(key_comp(key(x),k)) {
		//			y = x;
		//			x = x->left;
		//		}else {
		//			x = x->right;
		//		}
		//	}
		//	iterator j
		//}

		void remove(value_type val) {
			link_type it = _find(val);
			if(it==nullptr) return;
			_remove(it);
			--node_count;

		}



		//void _remove(link_type z) {
		//	link_type y = z, x;
		//	color_type ori_color = y->color;
		//	bool is_left= z->parent->left==z ? true:false ;
		//	if(z->left==nullptr) {
		//		x = z->right;
		//		_transplant(z, z->right);
		//	}else if(z->right==nullptr) {
		//		x = z->left;
		//		_transplant(z, z->left);
		//	} else {
		//		y = min(z->right);
		//		ori_color = y->color;
		//		x = y->right;
		//		if(y->parent==z) {
		//			x->parent = y;
		//		} else {
		//			_transplant(y, y->right);
		//			y->right = z->right;
		//			y->right->parent = y;
		//		}
		//		_transplant(z, y);
		//		y->left = z->left;
		//		y->left->parent = y;
		//		y->color = z->color;
		//	}
		//	if (ori_color == BLACK) {
		//		link_type p = z->parent;
		//		if(x==nullptr&&is_left) {
		//			link_type u = p->right;
		//			if(u==nullptr) {
		//				z->parent->color = RED;
		//				p = z->parent;
		//				remove_aux(p);
		//				return;
		//			}
		//			if (colorOf(u) == RED) {
		//				left_rotate(p);
		//				u->color = BLACK;
		//				p->color = RED;
		//				u = p->right;
		//			}
		//			if (colorOf(u->left) == BLACK && colorOf(u->right) == BLACK) {
		//				u->color = RED;
		//				
		//			}
		//			else if (colorOf(u->right) == BLACK) {
		//				u->left->color = BLACK;
		//				u->color = RED;
		//				right_rotate(u);
		//				u = p->right;
		//			}
		//			u->color = colorOf(p);
		//			p->color = BLACK;
		//			if(u->right!=nullptr)
		//			u->right->color = BLACK;
		//			left_rotate(p);
		//			p = root();
		//		}else if(x==nullptr&&!is_left) {
		//			link_type u = p->left;  //cannot be nullptr
		//			if (u == nullptr) {
		//				z->parent->color = BLACK;
		//				return;
		//			}
		//			if (colorOf(u) == RED) {
		//				right_rotate(p);
		//				u->color = BLACK;
		//				p->color = RED;
		//				u = p->left;
		//			}
		//			if (colorOf(u->left) == BLACK && colorOf(u->right) == BLACK) {
		//				u->color = RED;
		//			}
		//			else if (colorOf(u->left) == BLACK) {
		//				u->right->color = BLACK;
		//				u->color = RED;
		//				left_rotate(u);
		//				u = p->left;
		//			}
		//			u->color = colorOf(p);
		//			p->color = BLACK;
		//			if (u->left != nullptr)
		//			u->left->color = BLACK;
		//			right_rotate(p);
		//			p = root();
		//		}
		//		remove_aux(p);
		//	}

		//}

		void _remove(link_type node) {
			{
				link_type child, parent;
				color_type color;

				if ((node->left != NULL) && (node->right != NULL))
				{
					link_type replace = node;

					replace = min(replace->right);

					if (node!=root())
					{
						if (node->parent->left == node)
							node->parent->left = replace;
						else
							node->parent->right = replace;
					}
					else {
						root() = replace;
					}
					child = replace->right;
					parent = replace->parent;
					color = replace->color;

					if (parent == node)
					{
						parent = replace;
					}
					else
					{
						if (child!=nullptr)
							child->parent = parent;
						parent->left = child;

						replace->right = node->right;
						node->right->parent= replace;
					}

					replace->parent = node->parent;
					replace->color = node->color;
					replace->left = node->left;
					node->left->parent = replace;

					if (color == BLACK)
						remove_aux(child, parent);

					destory_node(node);
					return;
				}
				if (node->left != NULL)
					child = node->left;
				else
					child = node->right;

				parent = node->parent;
				color = node->color;

				if (child!=nullptr)
					child->parent = parent;

				if (parent!=header)
				{
					if (parent->left == node)
						parent->left = child;
					else
						parent->right = child;
				}
				else {
					root() = child;
				}
				if (color == BLACK)
					remove_aux(child, parent);
				delete node;
			}
		}

		void remove_aux(link_type& it,link_type& par) {
			while (it!=root()&&colorOf(it)==BLACK) {
				if (it == par->left) {
					link_type u = par->right;
					if (colorOf(u) == RED) {
						left_rotate(par);
						u->color = BLACK;
						par->color = RED;
						u = par->right;
					}
					if (colorOf(u->left) == BLACK && colorOf(u->right) == BLACK) {
						u->color = RED;
						it = par;
						par = it->parent;
					}
					else {
						if (colorOf(u->right) == BLACK) {
							u->left->color = BLACK;
							u->color = RED;
							right_rotate(u);
							u = par->right;
						}
						u->color = colorOf(par);
						par->color = BLACK;
						u->right->color = BLACK;
						left_rotate(par);
						it = root();
						break;
					}
				}
				else {
					link_type u = par->left;
					if (colorOf(u) == RED) {
						right_rotate(par);
						u->color = BLACK;
						par->color = RED;
						u = par->left;
					}
					if (colorOf(u->left) == BLACK && colorOf(u->right) == BLACK) {
						u->color = RED;
						it = par;
						par = it->parent;
					}
					else {
						if (colorOf(u->left) == BLACK) {
							u->right->color = BLACK;
							u->color = RED;
							left_rotate(u);
							u = par->left;
						}
						u->color = colorOf(par);
						par->color = BLACK;
						u->left->color = BLACK;
						right_rotate(par);
						it = root();
						break;
					}
				}

			}
			if(it!=nullptr)
			it->color = BLACK;
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
	public:
		void print(int depth=4) {
			std::queue<link_type> stack1,stack2;
			stack1.push(root());
			bool flag = true;
			while(!stack1.empty()||!stack2.empty()) {
				std::queue<link_type>* st1,* st2;

				if(flag) {
					st1 = &stack1;
					st2 = &stack2;
				} else {
					st1 = &stack2;
					st2 = &stack1;
				}
				
				for (int i = depth; i >= 0; --i) std::cout << "   ";
				--depth;
				while(!st1->empty()) {
					if (st1->front() == nullptr) std::cout << "   ";
					else {
						link_type tmp = st1->front();
						if(tmp->color==BLACK) {
							std::cout << "/" << tmp->value << "\\";
						}else {
							std::cout << " " << tmp->value << " ";
						}
						st2->push(tmp->left);
						st2->push(tmp->right);
					}
					st1->pop();
				}
				flag = !flag;
				std::cout << std::endl;
			}
		}
		bool test() {
			if (root() == nullptr) return true;
			if(color(root())==RED) return false;
			int max_black = 0;
			std::queue<std::pair<link_type,int>> st;
			st.push(std::make_pair(root(),0));
			while(!st.empty()) {
				link_type it = st.front().first;
				int cur_black = st.front().second;
				if (it->color == BLACK) cur_black++;
				if(it->color==RED) {
					if(colorOf(it->left)!=BLACK||colorOf(it->right)!=BLACK) return false;
				}
				if (it->left != nullptr) st.push(std::make_pair(it->left, cur_black));
				if (it->right != nullptr) st.push(std::make_pair(it->right, cur_black));
				if(it->left==nullptr&&it->right==nullptr) {
					if (max_black == 0) max_black = cur_black;
					else if(max_black!=cur_black) return false;
				}
				st.pop();
			}
			return true;
		}

	};

	template <typename T>
	struct identity
	{
		T operator()(T x) const { return x; }
	};
}


#endif
