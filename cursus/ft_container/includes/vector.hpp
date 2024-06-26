#ifndef CONTAINER_HPP
# define CONTAINER_HPP

# include <iostream>
# include "iterators/vector_iterator.hpp"
# include "iterators/reverse_iterator.hpp"
# include "utils/utils.hpp"

namespace ft
{
	template <class _Tp, class _Allocator = std::allocator<_Tp> >
	class vector
	{
		/*************************************/
		/*           Define Types            */
		/*************************************/

		public:
			/* Type define */
			typedef _Tp                     value_type;
			typedef _Allocator              allocator_type;
			typedef size_t                  size_type;
			typedef long int                difference_type;

			typedef value_type*             pointer;
			typedef const value_type*       const_pointer;
			typedef value_type&             reference;
			typedef const value_type&       const_reference;

			/* Ieterator define */
			// typedef pointer             	iterator;
			// typedef const_pointer			const_iterator;
			typedef typename ft::vector_iterator<pointer>			iterator;
			typedef typename ft::vector_iterator<const_pointer>		const_iterator;
			typedef typename ft::reverse_iterator<iterator>     	reverse_iterator;
			typedef typename ft::reverse_iterator<const_iterator>   const_reverse_iterator;

		/*************************************/
		/*         Member Variables          */
		/*************************************/

		private:
			allocator_type  _alloc;
			pointer         _begin;
			pointer			_end;
			pointer			_end_cap;
			
		/*************************************/
		/*          Public Function          */
		/*************************************/

		public:

			/*************************************/
			/*            Constructor            */
			/*************************************/

			vector(const allocator_type &alloc = allocator_type())
			: _alloc(alloc), _begin(u_nullptr), _end(u_nullptr), _end_cap(u_nullptr) {}

			vector(size_type n, const_reference val = value_type(), const allocator_type &alloc = allocator_type())
			: _alloc(alloc)
			{
				this->vallocate(n);
				this->vconstruct(_end + n, val);
			}

			template <class InputIterator>
			vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type(),
					typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type * = u_nullptr)
			: _alloc(alloc)
			{
				size_type n = 0;
				for (InputIterator it = first; it != last; it++)
					n++;
				this->vallocate(n);
				for (size_type i = 0; i < n; i++)
					this->_alloc.construct(this->_end++, *(first++));
			}

			// 참조자 빼고 호출하면 복사 생성자가 호출되는데 메모리 이중 해제 이슈 있음
			vector(const vector &cls)
			: _alloc(cls._alloc), _begin(u_nullptr), _end(u_nullptr), _end_cap(u_nullptr)
			{
				size_type n = cls.size();

				if (n > 0) {
					this->vallocate(n);
					for (const_iterator it = cls.begin(); it < cls.end(); it++)
						this->_alloc.construct(this->_end++, *it);
					// this->vconstruct(this->_end + n, cls.begin());
				}
			}

			/*************************************/
			/*             Destructor            */
			/*************************************/

			~vector() { _alloc.deallocate(this->_begin, this->capacity()); };

			/*************************************/
			/*              Operator             */
			/*************************************/

			vector &operator=(const vector<value_type> &cls)
			{
				if (this != &cls) {
					if (this->capacity() < cls.size())
						this->realloc(cls.size());
					this->clear();
					this->insert(this->begin(), cls.begin(), cls.end());
				}

				return *this;
			}

			/*************************************/
			/*             Iterators             */
			/*************************************/

			iterator begin() { return this->_begin; }

			const_iterator begin() const { return this->_begin; }

			iterator end() { return this->_end; }

			const_iterator end() const { return this->_end; }

			reverse_iterator rbegin() { return reverse_iterator(this->end()); }

			const_reverse_iterator rbegin() const { return const_reverse_iterator(this->end()); }

			reverse_iterator rend() { return reverse_iterator(this->begin()); }

			const_reverse_iterator rend() const { return const_reverse_iterator(this->begin()); }

			/*************************************/
			/*              Capacity             */
			/*************************************/
			
			size_type   size() const { return this->_end - this->_begin; }

			size_type   max_size() const { return std::min<size_type>(_alloc.max_size(), std::numeric_limits<difference_type>::max()); }

			size_type   capacity() const { return this->_end_cap - this->_begin; }

			bool	empty() const { return this->size() == 0; };
			
			void	resize(size_type n, value_type val = value_type())
			{
				size_type cs = this->size();
				if (cs < n)
					this->append(n - cs, val);
				else if (cs > n)
					this->erase(this->begin() + n, this->end());
			}

			void	reserve(size_type n)
			{
				if (n > this->capacity()) {
					pointer p_begin = this->_begin;
					size_type p_size = this->size();
					size_type p_cap = this->capacity();
					this->vallocate(n);
					vconstruct(_begin + p_size, p_begin);
					this->_alloc.deallocate(p_begin, p_cap);
				}
				// if (n > this->max_size())
				// 	throw std::out_of_range("Out of range!!");
				// if (n > this->capacity())
				// 	this->realloc(n);
			}

			/*************************************/
			/*           Element access          */
			/*************************************/

			reference operator[](size_type n)
			{
				if (n < 0 || n > this->max_size())
					throw std::out_of_range("Out of range!!");
				return this->begin()[n];
			}

			const_reference operator[](size_type n) const { return *(this->_begin + n); }

			reference at (size_type n)
			{
				if (n >= this->size())
					throw std::out_of_range("Out of range");
				return *(this->begin() + n);
			}

			const_reference at (size_type n) const
			{
				if (n >= this->size())
					throw std::out_of_range("Out of range");
				return *(this->begin() + n);
			}

			reference front() { return *(this->begin()); }

			const_reference front() const { return *(this->begin()); }

			reference back() { return *(this->end() - 1); }

			const_reference back() const { return *(this->end() - 1); }

			/*************************************/
			/*             Modifiers             */
			/*************************************/

			/* assign */
			template <class InputIterator> 
			void assign(InputIterator first, InputIterator last,
					typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = u_nullptr)
			{
				this->clear();
				this->insert(this->begin(), first, last);
			};

			void assign(size_type n, const_reference val)
			{
				if (n > this->capacity())
					this->realloc(n);
				this->clear();
				this->vconstruct(this->_end + n, val);
			}

			/* push_back */
			void push_back(const_reference val)
			{

				if (this->size() == this->capacity())
					this->realloc(recommand_cap(this->size() + 1));
				this->vconstruct(this->_end + 1, val);
			}

			/* pop_back */
			void pop_back() { this->_alloc.destroy(--(this->_end)); }

			/* insert */
			iterator insert(iterator position, const_reference val)
			{
				size_type pos = static_cast<size_type>(&(*position) - &(*this->begin()));
				this->move(1, position, this->end());
				this->_alloc.construct((this->begin() + pos).base(), val);
				return this->begin() + pos;
			}

			void insert(iterator position, size_type n, const_reference val)
			{
				size_type pos = static_cast<size_type>(&(*position) - &(*this->begin()));
				this->move(n, position, this->end());
				for (size_type i = 0; i < n; i++)
					this->_alloc.construct(&(*(this->begin() + pos + i)), val);
			}

			template <class InputIterator>
			void insert(iterator position, InputIterator first, InputIterator last,
					typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type * = u_nullptr)
			{
				size_type pos = static_cast<size_type>(&(*position) - &(*this->begin()));
				size_type n = 0;
				for (InputIterator it = first; it != last; it++)
					n++;
				this->move(n, position, this->end());
				for (size_type i = 0; i < n; i++, first++)
					this->_alloc.construct(&(*(this->begin() + pos + i)), *first);
			}

			/* erase */
			iterator erase(iterator position)
			{
				iterator last = position + 1;
				return erase(position, last);
			}

			iterator erase(iterator first, iterator last)
			{
				iterator p = first;
				for (; p < last; p++)
					this->_alloc.destroy(&(*p));
				for (int i = 0; i < _end - &(*last); i++) {
					_alloc.construct(&(*(first + i)), *(last + i));
					_alloc.destroy(&(*(last + i)));
				}
				this->_end -= (&(*last) - &(*first));
				return first;
			}

			/* swap */
			void swap(vector &cls)
			{
				if (this == &cls)
					return ;
				std::swap(this->_alloc, cls._alloc);
				std::swap(this->_begin, cls._begin);
				std::swap(this->_end, cls._end);
				std::swap(this->_end_cap, cls._end_cap);
			}

			/* clear */
			void clear()
			{
				for (; this->_begin != this->_end; )
					this->pop_back();
			}


			/*************************************/
			/*             Allocator             */
			/*************************************/

			allocator_type get_allocator() const { return this->_alloc; }
		

		/*************************************/
		/*         Private Function          */
		/*************************************/
		private:

			void vconstruct(pointer p, const_reference val)
			{
				for (; _end < p; _end++)
					this->_alloc.construct(_end, val);
			}

			// template <class Iterator>
			void vconstruct(iterator p, iterator val)
			{
				for (; _end != &(*p); _end++, val++) {
					this->_alloc.construct(_end, *val);
				}
			}

			void vallocate(size_type n)
			{
				if (n > this->max_size())
					throw std::out_of_range("Out of range");
				this->_begin = this->_end = _alloc.allocate(n);
				this->_end_cap = this->_begin + n;
			}

			void vdestruct(iterator first, iterator last)
			{
				for (; first < last; first++) {
					this->_alloc.destroy(first);
					this->_alloc.deallocate(first, 1);
				}
			}

			void vdestruct(iterator p)
			{
				for (; _end >= p; --_end) {
					this->_alloc.destroy(this->_end);
					this->_alloc.deallocate(this->_end, 1);
				}
			}

			void vdeallocate() { _alloc.deallocate(this->_begin, this->capacity()); }

			void realloc(size_type cap)
			{
				vector tmp;

				this->swap(tmp);
				this->vallocate(cap);
				if (tmp._begin != NULL) 
					this->vconstruct(this->_end + tmp.size(), tmp._begin);
			}

			void append(size_type n, const_reference val)
			{
				size_type additional = static_cast<size_type>(this->_end_cap - this->_end);
				if (additional < n)
					this->realloc(recommand_cap(n + this->size()));
				this->vconstruct(&(*(this->end() + n)), val);
			}

			template <class InputIterator>
			void move(size_type n, InputIterator first, InputIterator last)
			{
				size_type pos = static_cast<size_type>(&(*first) - &(*this->begin()));
				size_type move_size = static_cast<size_type>(&(*last) - &(*first));
				pointer tmp = _alloc.allocate(move_size);

				if (this->size() + n > this->capacity()) {
					this->realloc(recommand_cap(n + this->size()));
					first = this->begin() + pos;
				}
				for (size_type i = 0; i < move_size; i++)
					tmp[i] = first[i];
				for (size_type i = 0; i < move_size; i++) {
					*(this->begin() + pos + n + i) = tmp[i];
					this->_alloc.destroy(&(*(this->begin() + pos + i)));
				}
				this->_end += n;
				this->_alloc.deallocate(tmp, move_size);
			}

			// size_type new_capacity(size_type n)
			// {
			// 	size_type new_cap = this->capacity() == 0 ? 1 : this->capacity();
			// 	while (new_cap < (this->size() + n))
			// 		new_cap *= 2;
			// 	return new_cap;
			// }

			size_type recommand_cap(size_type n)
			{
				const size_type ms = this->max_size();
				if (n > ms)
					throw std::out_of_range("Out of range");
				const size_type cap = this->capacity();
				if (cap >= ms / 2)
					return ms;
				return std::max(2 * cap, n);
			}
	};

	template <class T, class Alloc>
	bool operator== (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return (lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin())); }

	template <class T, class Alloc>
	bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return !(lhs == rhs); }

	template <class T, class Alloc>
	bool operator<  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

	template <class T, class Alloc>
	bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return !(rhs < lhs); }

	template <class T, class Alloc>
	bool operator>  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return rhs < lhs; }

	template <class T, class Alloc>
	bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{ return !(lhs < rhs); }

	template <class T, class Alloc>
  	void swap (vector<T,Alloc>& x, vector<T,Alloc>& y)
	{ x.swap(y); }
}

#endif