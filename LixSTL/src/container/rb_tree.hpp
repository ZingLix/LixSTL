#ifndef RBTREE_H_
#define RBTREE_H_
#include "../traits/iterator_traits.hpp"
#include "../memory/allocator.hpp"
#include "../memory/allocator_traits.hpp"
#include <queue>
#include "queue.hpp"

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

		void increment()  {
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
		void decrement()  {
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

		T& operator*() {
			return static_cast<link_type>(node)->value;
		}
		T operator->() {
			return &(operator*());
		}
		self operator++()  {
			increment();
			return *this;
		}
		self operator++(int)  {
			self tmp = *this;
			increment();
			return tmp;
		}
		self operator--()  {
			decrement();
			return *this;
		}
		self operator--(int) {
			self tmp = *this;
			decrement();
			return tmp;
		}
		bool operator==(const self& x) const { return node == x.node; }
		bool operator!=(const self& x) const { return node != x.node; }
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
		using self = rb_tree<Value, Compare, Alloc>;

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
		template< class... Args >
		link_type emplace_node(Args&&... args) {
			link_type node = get_node();
			allocator_traits<node_allocator>::construct(alloc_, node, args);
			return node;
		}
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
		template< class InputIt >
		rb_tree(InputIt first, InputIt last,
			bool is_unique = false,
			const Compare& comp = Compare(),
			const Alloc& alloc = Alloc()):alloc_(alloc),node_count(0),comp(comp) {
			init();
			if(is_unique) insert_unique(first, last);
			else insert(first, last); 
		}
		rb_tree(rb_tree& other) {
			init();
			copy(other.root());
			node_count = other.node_count;
		}

		rb_tree(std::initializer_list<value_type> ilist,
			bool is_unique = false,
			const Compare& comp = Compare(),
			const Alloc& alloc = Alloc()):alloc_(alloc),comp(comp) {
			init();
			if (is_unique) insert_unique(ilist);
			else insert(ilist);
		}

		~rb_tree() {
			clear(root());
			put_node(header);
		}
		Compare key_comp() const { return comp; }
		iterator begin()  { return leftmost(); }
		iterator end() { return iterator(header); }
		bool empty() const { return node_count == 0; }
		size_type size() const { return node_count; }
		node_allocator get_allocator() const { return alloc_; }

		void operator=(const rb_tree& other) {
			clear();
			copy(other.root());
		}

		void operator=(std::initializer_list<value_type> ilist) {
			clear();
			insert(ilist);
		}

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

		void copy(link_type root) {
			header->parent = _copy_tree(root, header);
			update_leftmost();
			update_rightmost();
		}

		link_type _copy_tree(link_type root,link_type parent) {
			if (root == nullptr) return nullptr;
			link_type node = clone_node(root);
			node->left = _copy_tree(root->left,node);
			node->right = _copy_tree(root->right,node);
			node->parent = parent;
			return node;
		}

	public:
		void clear() noexcept {
			node_count = 0;
			clear(root());
		}

	protected:
		void clear(link_type it) noexcept {
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
			header->parent = nullptr;
		}

	public:
		iterator insert(const value_type& x) {
			iterator p;
			link_type it = root();
			if(it==nullptr) {
				++node_count;
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

		void insert(std::initializer_list<value_type> ilist) {
			for (auto it = ilist.begin(); it != ilist.end(); ++it) {
				insert(*it);
			}
		}

		template< class InputIt >
		void insert(InputIt first, InputIt last) {
			while(first!=last) {
				insert(*first);
				++first;
			}
		}

		template< class... Args >
		iterator emplace(Args&&... args) {
			link_type node = emplace_node(args);
			iterator p;
			link_type it = root();
			if (it == nullptr) {
				return iterator(node);
			}
			else {
				link_type tmp = parent(it);
				while (it != nullptr) {
					tmp = it;
					if (comp(node->value, value(it)))
						it = left(it);
					else { it = right(it); }
				}
				p = _insert(tmp, node);
			}
			++node_count;
			return p;
		}

		std::pair<iterator,bool> insert_unique(const value_type& x) {
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

		void insert_unique(std::initializer_list<value_type> ilist) {
			for(auto it=ilist.begin();it!=ilist.end();++it) {
				insert_unique(*it);
			}
		}

		template< class InputIt >
		void insert_unique(InputIt first, InputIt last) {
			while (first != last) {
				insert_unique(*first);
				++first;
			}
		}

		template< class... Args >
		std::pair<iterator, bool> emplace_unique(Args&&... args) {
			link_type node = emplace_node(args);
			iterator p;
			link_type it = root();
			++node_count;
			if (it == nullptr) {
				return std::make_pair(node, true);
			}
			else {
				link_type tmp = parent(it);
				while (it != nullptr) {
					tmp = it;
					if (comp(node->value, value(it)))
						it = left(it);
					else { it = right(it); }
				}
				link_type t = tmp;
				while (t != header) {
					if (!(comp(t->value, node->value) || comp(node->value, t->value))) {
						--node_count;
						return std::make_pair(iterator(tmp), false);
					}
					t = t->parent;
				}
				p = _insert(tmp, node);
			}
			return std::make_pair(p, true);
		}

	protected:
		iterator _insert(link_type x,const value_type& val) {
			auto node = create_node(val);
			return _insert(x,node);
		}

		iterator _insert(link_type x, link_type node) {
			node->parent = x;
			if (!comp(value(x), node->value)) {
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

		iterator create_root(const value_type& val) {
			auto node = create_node(val);
			header->parent = node;
			node->parent = header;
			leftmost() = node;
			rightmost() = node;
			node->left = node->right = nullptr;
			node->color = BLACK;
			return iterator(node);
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
		iterator find(value_type val) {
			return iterator(_find(val));
		}

	protected:
		link_type _find(value_type val) {
			return _find(val, root());
		}

		link_type _find(value_type val,link_type begin) {
			link_type it = begin;
			while (it != nullptr) {
				if (comp(val, it->value)) {
					it = it->left;
				}
				else if (equal(val,it->value)) {
					return it;
				}
				else {
					it = it->right;
				}
			}
			return it;
		}

	public:
		size_type erase(value_type val) {
			//queue<link_type> q;
			//link_type it = _find(val);
			//while(it!=nullptr&&it!=header&& equal(it->value,val)) {
			//	q.push(it);
			//	it = successor(it);
			//}
			//size_type count = q.size();
			////if(it==nullptr) return end();
			//while (!q.empty()) {
			//	_remove_entry(q.front());
			//	q.pop();
			//}
			//return count;
			link_type it = _find(val);
			if (it == nullptr) return 0;
			_remove_entry(it);
			return 1;
		}

		iterator erase(iterator it) {
			return _remove_entry(it.node);
		}

		iterator erase(const_iterator first, const_iterator last) {
			size_type distance = 0;
			iterator ifirst=first, ilast=last;
			while (ifirst!=ilast) {
				++distance;
				++ifirst;
			}
			iterator it = first;
			for(int i=0;i<distance-1;++i) {
				erase(it++);
			}
			return erase(it);
		}

	protected:
		iterator _remove_entry(link_type node)
		{
			if (node_count == 0) return end();
			link_type succ = successor(node);
			_remove(node);
			if (node == leftmost()) update_leftmost();
			if (node == rightmost()) update_rightmost();
			--node_count;
			return iterator(succ);
		}

		void _remove(link_type node) {
			{
				link_type child, parent;
				color_type color;
				if(node==header&&node->parent->left==nullptr&&node->parent->right==nullptr) {
					destory_node(node->parent);
					node->parent = nullptr;
					update_leftmost();
					update_rightmost();
					return;
				}
				if ((node->left != nullptr) && (node->right != nullptr))
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

		link_type successor(const link_type& node) {
			link_type p = node;
			if (p->right != nullptr)
				return min(p->right);
			auto y = p->parent;
			while ( y!=nullptr && p==y->right ) {
				p = y;
				y = y->parent;
			}
			return y;
		}

		link_type predecessor(const link_type& node) {
			link_type p = node;
			if (p->left != nullptr)
				return max(p->left);
			auto y = p->parent;
			while (y != nullptr && p == y->left) {
				p = y;
				y = y->parent;
			}
			return y;
		}

		void update_leftmost() {
			link_type it = root();
			if(it!=nullptr) {
				while (it->left!=nullptr) {
					it = it->left;
				}
				header->left = it;
			} else {
				header->left = header;
			}
		}

		void update_rightmost() {
			link_type it = root();
			if (it != nullptr) {
				while (it->right != nullptr) {
					it = it->right;
				}
				header->right = it;
			}
			else {
				header->right = header;
			}
		}

	public:
		size_type count(const value_type& val) const {
			int count = 0;
			for(auto it=begin();it!=end();++it) {
				if (euqal(*it, val)) ++count;
			}
			return count;
		}
	protected:
		bool equal(const value_type& val1,const value_type& val2) {
			return !comp(val1, val2) && !comp(val1, val2);
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
			for(auto it =begin();it!=end();++it) std::cout <<*it<< " ";
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


	protected:
		node_allocator alloc_;
		size_type node_count;
		link_type header;
		Compare comp;
	};
}


#endif
