#ifndef VECTOR_H_
#define VECTOR_H_

#include "../../include/memory"
#include <cstddef>
#include <algorithm>
#include <stdexcept>

namespace lix
{
	template<class T, class Alloc = allocator<T>>
	class vector
	{
	public:
		using value_type = T;
		using allocator_type = Alloc;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type & ;
		using const_reference = const value_type&;
		using pointer = typename allocator_traits<Alloc>::pointer;
		using const_pointer = typename allocator_traits<Alloc>::const_pointer;
		using iterator = value_type * ;
		using const_iterator = const iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		vector() noexcept(noexcept(Alloc())) :allocator_(), _start(nullptr), _end(nullptr), _tail(nullptr) {}
		explicit vector(const Alloc& alloc) noexcept : allocator(alloc), _start(nullptr), _end(nullptr), _tail(nullptr) {	}
		vector(size_type n, const T& value, const Alloc& alloc = Alloc()) {
			allocator_ = alloc;
			fill_initialize(n, value);
		}
		vector(size_type n, const Alloc& alloc = Alloc()) :allocator_(alloc) { fill_initialize(n, T()); }
		template< class InputIt >
		vector(InputIt first, InputIt last, const Alloc& alloc = Alloc()) {
			allocator_ = alloc;
			size_type count = last - first;
			_start = allocator_traits<Alloc>::allocate(allocator_, count);
			_end = _start + count;
			_tail = _end;
			for (auto itr = _start; first != last; ++itr, ++first) {
				allocator_traits<Alloc>::construct(allocator_, itr, *first);
			}
		}
		vector(int n, const T& value) { fill_initialize(n, value); }
		vector(const vector<T, Alloc>& vec);
		vector(const vector<T, Alloc>& vec, const Alloc& alloc);
		vector(vector&& other) noexcept;
		vector(vector&& vec, const Alloc& alloc) {
			if (alloc == vec.get_allocator()) {
				allocator_ = vec.get_allocator();
				_start = vec._start;
				_end = vec._end;
				_tail = vec._end;

			}
			else {
				//TODO 
				/*allocator_ = vec.get_allocator();
				_start = allocator::allocate(vec.size());
				_end = _start + vec.size();
				_tail = _end;
				auto tmp = _start;
				auto it = vec.begin();
				for (; it != vec.end(); ++it, ++tmp) tmp = std::move(it);*/
			}
			vec._start = nullptr;
			vec._end = nullptr;
			vec._tail = nullptr;
		}
		vector(std::initializer_list<T> init,
			const Alloc& alloc = Alloc()) {
			allocator_ = alloc;
			_start = allocator::allocate(init.size());
			_end = _start + init.size();
			_tail = _end;
			auto it = init.begin();
			auto it2 = _start;
			for (; it != init.end(); ++it, ++it2) {
				allocator_traits<Alloc>::construct(allocator_, it2, *it);
			}
		}
		~vector() {
			_clean();
		}

		vector<T, Alloc>& operator=(const vector<T, Alloc>& vec);
		vector& operator=(vector&& other) noexcept(allocator_traits<Alloc>::propagate_on_container_move_assignment::value
			|| allocator_traits<Alloc>::is_always_equal::value);
		vector& operator=(std::initializer_list<T> ilist);

		void assign(size_type count, const T& value);
		template< class InputIt, std::enable_if_t<std::is_base_of<std::input_iterator_tag,
			typename std::iterator_traits<InputIt>::iterator_category>::value, int> = 0 >
			void assign(InputIt first, InputIt last);
		void assign(std::initializer_list<T> ilist);

		allocator_type get_allocator() const {
			return allocator_;
		}

		reference at(size_type pos) {
			if (pos >= size()) throw std::out_of_range("out of range");
			return *(_start + pos);
		}
		const_reference at(size_type pos) const {
			if (pos >= size()) throw std::out_of_range("out of range");
			return *(_start + pos);
		};
		reference operator[](size_type n) { return *(_start + n); }

		reference front() { return *_start; }
		reference back() { return *(_end - 1); }
		T* data() noexcept { return _start; }
		const T* data() const noexcept { return _start; }

		iterator begin() const { return _start; }
		iterator end() const { return _end; }

		bool empty() { return _start == _end; }
		size_type size() const { return static_cast<size_type>(_end - _start); }
		size_type max_size() const noexcept { return static_cast<size_type>(-1); }
		void reserve(size_type new_cap);
		size_type capacity() const noexcept { return static_cast<size_type>(_tail - _start); }
		void shrink_to_fit();

		void clear();
		void insert(iterator pos, size_type n, const T& value);
		template< class... Args >
		iterator emplace(const_iterator pos, Args&&... args);
		template< class... Args >
		reference emplace_back(Args&&... args);
		iterator erase(iterator first, iterator last);
		iterator erase(iterator pos);

		void push_back(const T& x);
		void pop_back();

		void resize(size_type count);
		void resize(size_type count, const value_type& value);
		void swap(vector& other) noexcept(allocator_traits<Alloc>::propagate_on_container_swap::value
			|| allocator_traits<Alloc>::is_always_equal::value);

	protected:
		void deallocate();
		iterator allocate_and_fill(size_type n, const T& x);
		void fill_initialize(size_type n, const T& value);
		void reallocate(size_type count);

		void insert_aux(iterator pos, size_type n, const T& value);
		void insert_aux(iterator pos, const T& value);

		void _clean() {
			for (auto itr = _start; itr != _end; ++itr) {
				allocator_traits<Alloc>::destroy(allocator_, itr);
			}
			allocator_traits<Alloc>::deallocate(allocator_, _start, _tail - _start);
		}

	private:
		typedef Alloc allocator;
		Alloc allocator_;
		iterator _start; //数据头
		iterator _end;   //数据尾
		iterator _tail;  //空间尾
	};


	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator pos) {
		if (pos + 1 != _end) {
			std::copy(pos + 1, _end, pos);
		}
		--_end;
		allocator_traits<Alloc>::destroy(allocator_, _end);
		return pos;
	}

	template <class T, class Alloc>
	template <class ... Args>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::emplace(const_iterator pos, Args&&... args) {
		allocator_traits<Alloc>::destroy(allocator_, pos);
		allocator_traits<Alloc>::construct(allocator_, pos, args...);
		return pos;
	}

	template <class T, class Alloc>
	template <class ... Args>
	typename vector<T, Alloc>::reference vector<T, Alloc>::emplace_back(Args&&... args) {
		if (_end == _tail) {
			reserve(size() == 0 ? 1 : size() * 2);
		}
		allocator_traits<Alloc>::construct(allocator_, _end, args...);
		auto tmp = _end;
		++_end;
		return *tmp;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::swap(vector& other) noexcept(allocator_traits<Alloc>::propagate_on_container_swap::value
		|| allocator_traits<Alloc>::is_always_equal::value) {
		if constexpr(allocator_traits<allocator_type>::propagate_on_container_swap::value == true) {
			auto tmp = allocator_;
			allocator_ = other.allocator_;
			other.allocator_ = tmp;
		}

		auto tmp_start = _start;
		auto tmp_end = _end;
		auto tmp_tail = _tail;
		_start = other._start;
		_end = other._end;
		_tail = other._tail;
		other._start = tmp_start;
		other._end = tmp_end;
		other._tail = tmp_tail;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::resize(size_type count) {
		if (count < size()) {
			const size_t new_size = count;
			auto new_start = allocator::allocate(new_size);
			auto new_end = lix::uninitialized_copy(_start, _start + count, new_start);
			_clean();
			_start = new_start;
			_end = new_end;
			_tail = _start + new_size;
		}
		else if (count > capacity()) {
			const size_t old_size = size();
			reserve(count);
			for (int i = 0; i < count - old_size; ++i, ++_end) {
				allocator_traits<Alloc>::construct(allocator_, _end);
			}
		}
		else {
			for (; _end != _start + count; ++_end) {
				allocator_traits<Alloc>::construct(allocator_, _end);
			}
			shrink_to_fit();
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::resize(size_type count, const value_type& value) {
		if (count < size()) {
			const size_t new_size = count;
			auto new_start = allocator::allocate(new_size);
			auto new_end = lix::uninitialized_copy(_start, _start + count, new_start);
			_clean();
			_start = new_start;
			_end = new_end;
			_tail = _start + new_size;
		}
		else if (count > capacity()) {
			const size_t old_size = size();
			//const size_t new_size = count;
			reserve(count);
			for (int i = 0; i < count - old_size; ++i, ++_end) {
				allocator_traits<Alloc>::construct(allocator_, _end, value);
			}
		}
		else {
			for (; _end != _start + count; ++_end) {
				allocator_traits<Alloc>::construct(allocator_, _end, value);
			}
			shrink_to_fit();
		}
	}

	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
		iterator i = std::copy(last, _end, first);
		for (auto tmp = i; i != _end; ++i) {
			allocator_traits<Alloc>::destroy(allocator_, tmp);
		}
		_end = _end - (last - first);
		return first;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::insert(iterator pos, size_type n, const T& value) {
		if (n > 0) {
			if (static_cast<size_type>(_tail - _end) >= n) {
				auto elems_after = _end - pos;
				iterator old_end = _end;
				std::uninitialized_copy(_end - n, _end, _end);
				_end += n;
				std::copy_backward(pos, old_end - n, old_end);
				std::fill(pos, pos + n, value);
			}
			else {
				insert_aux(pos, n, value);
			}
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::pop_back() {
		--_end;
		allocator_traits<Alloc>::destroy(allocator_, _end);
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::reserve(size_type new_cap) {
		if (new_cap > capacity()) {
			const size_t old_size = size();
			auto new_start = allocator::allocate(new_cap);
			auto new_end = lix::uninitialized_copy(_start, _start + old_size, new_start);
			for (auto tmp = _start; tmp != _end; ++tmp)
				allocator_traits<Alloc>::destroy(allocator_, tmp);
			deallocate();
			_start = new_start;
			_end = new_end;
			_tail = _start + new_cap;
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::shrink_to_fit() {
		const size_t old_size = size();
		const size_t new_cap = size();
		auto new_start = allocator::allocate(new_cap);
		auto new_end = lix::uninitialized_copy(_start, _start + old_size, new_start);
		for (auto tmp = _start; tmp != _end; ++tmp)
			allocator_traits<Alloc>::destroy(allocator_, tmp);
		deallocate();
		_start = new_start;
		_end = new_end;
		_tail = _start + new_cap;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::clear() {
		for (auto tmp = _start; tmp != _end; ++tmp)
			allocator_traits<Alloc>::destroy(allocator_, tmp);
		_end = _start;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::push_back(const T& x) {
		if (_end == _tail) {
			insert_aux(end(), x);
		}
		else {
			allocator_traits<Alloc>::construct(allocator_, _end, x);
			++_end;
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator pos, size_type n, const T& value) {
		const size_t old_size = size();
		const size_t new_size = old_size == 0 ? 1 : old_size * 2;
		auto new_start = allocator::allocate(new_size);
		auto new_end = lix::uninitialized_copy(_start, pos, new_start);
		while (n--) {
			allocator_traits<Alloc>::construct(allocator_, new_end, value);
			++new_end;
		}
		new_end = lix::uninitialized_copy(pos, _end, new_end);
		for (auto tmp = _start; tmp != _end; ++tmp)
			allocator_traits<Alloc>::destroy(allocator_, tmp);
		deallocate();
		_start = new_start;
		_end = new_end;
		_tail = _start + new_size;
	}

	template <class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::allocate_and_fill(size_type n, const T& x) {
		iterator address = allocator::allocate(n);
		lix::uninitialized_fill_n(address, n, x);
		return address;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::deallocate() {
		if (_start != nullptr) {
			allocator::deallocate(_start, _tail - _start);
		}
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator pos, const T& value) {
		insert_aux(pos, 1, value);
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::fill_initialize(size_type n, const T& value) {
		_start = allocate_and_fill(n, value);
		_end = _start + n;
		_tail = _end;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::reallocate(size_type count) {
		for (auto tmp = _start; tmp != _end; ++tmp)
			allocator_traits<Alloc>::destroy(allocator_, tmp);
		if (count > capacity()) {
			deallocate();
			const size_t new_size = count;
			auto new_start = allocator::allocate(new_size);

			_start = new_start;
			_end = _start + count;
			_tail = _end;
		}
		else {
			_end = _start;
		}
	}

	template <class T, class Alloc>
	vector<T, Alloc>::vector(const vector<T, Alloc>& vec) {
		reallocate(vec.size());
		auto tmp = _start;
		auto it = vec.begin();
		for (; it != vec.end(); ++it, ++tmp) allocator_traits<Alloc>::construct(allocator_, &*tmp, *it);
	}

	template <class T, class Alloc>
	vector<T, Alloc>::vector(const vector<T, Alloc>& vec, const Alloc& alloc) {
		allocator_ = alloc;
		reallocate(vec.size());
		auto tmp = _start;
		auto it = vec.begin();
		for (; it != vec.end(); ++it, ++tmp) allocator_traits<Alloc>::construct(allocator_, &*tmp, *it);
	}

	template <class T, class Alloc>
	vector<T, Alloc>::vector(vector&& other) noexcept {
		allocator_ = std::move(other.allocator_);
		_start = other._start;
		_end = other._end;
		_tail = other._tail;
		other._start = nullptr;
		other._end = nullptr;
		other._tail = nullptr;
	}

	template <class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc>& vec) {
		if constexpr(allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value == true) {
			allocator_ = vec.get_allocator();
		}
		reallocate(vec.size());
		auto tmp = _start;
		auto it = vec.begin();
		for (; it != vec.end(); ++_end, ++it) allocator_traits<Alloc>::construct(allocator_, _end, *it);
		return *this;
	}

	template <class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(vector<T, Alloc>&& vec)
		noexcept(allocator_traits<Alloc>::propagate_on_container_move_assignment::value
			|| allocator_traits<Alloc>::is_always_equal::value)
	{
		_clean();
		if constexpr(allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value == true) {
			allocator_ = vec.get_allocator();
			_start = vec.begin();
			_end = _start + vec.size();
			_tail = _start + vec.capacity();

		}
		else {
			if (allocator_ == vec.get_allocator()) {
				allocator_ = vec.get_allocator();
				_start = vec._start;
				_end = vec._end;
				_tail = vec._end;
			}
			else {
				allocator_ = vec.get_allocator();
				_start = allocator::allocate(vec.size());
				_end = _start + vec.size();
				_tail = _end;
				auto tmp = _start;
				auto it = vec.begin();
				for (; it != vec.end(); ++it, ++tmp)
					*tmp = std::move(*it);
			}
		}
		vec._start = nullptr;
		vec._end = nullptr;
		vec._tail = nullptr;
		return *this;
	}

	template <class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(std::initializer_list<T> ilist) {
		reallocate(ilist.size());
		auto it = ilist.begin();
		auto it2 = _start;
		for (; it != ilist.end(); ++it, ++it2) {
			allocator_traits<Alloc>::construct(allocator_, it2, *it);
		}
		return *this;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::assign(size_type count, const T& value) {
		reallocate(count);
		uninitialized_fill_n(begin(), count, value);
		_end = _start + count;
	}

#pragma warning(disable: 4544)
	template <class T, class Alloc>
	template <class InputIt, std::enable_if_t<std::is_base_of<std::input_iterator_tag,
		typename std::iterator_traits<InputIt>::iterator_category>::value, int> = 0 >
		void vector<T, Alloc>::assign(InputIt first, InputIt last) 
	{
		reallocate(last - first);
		auto it = begin();
		for (; first != last; ++first, ++it) {
			allocator_traits<Alloc>::construct(allocator_, it, *first);
		}
		_end = it;
	}

	template <class T, class Alloc>
	void vector<T, Alloc>::assign(std::initializer_list<T> ilist) {
		reallocate(ilist.size());
		for (auto it2 = ilist.begin(); it2 != ilist.end(); ++it2, ++_end) {
			allocator_traits<Alloc>::construct(allocator_, _end, *it2);
		}
	}
}



#endif
