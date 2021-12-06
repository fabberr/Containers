#ifndef NOSTL_ARR_ITERATORS_H
#define NOSTL_ARR_ITERATORS_H

/********** Headers **********/

// C library
#include <cstddef> // prtdiff_t

namespace nostl {

	/**
	 * Normal (bidirectional & random-access) iterator type for templated containers
	 * that use an underlying C-style array for storing data contiguously in memory,
	 * such that simple pointer arithmetics can be used to interate throught the 
	 * container.
	 * References an element stored in the container.
	*/
	template<typename _arr>
	class array_iterator {
	public:
		/********** Type Definitions **********/
		
		typedef typename _arr::value_type 	value_type; 		/** Type of values stored in the container. */
		typedef std::ptrdiff_t 				difference_type; 	/** Pointer difference type (for sum of pointers values in any address space). */
		typedef value_type* 				pointer; 			/** Pointer to value. */
		typedef value_type& 				reference; 			/** Reference to value. */
		
	protected:
		/********** Protected Members **********/
		
		pointer m_ptr; /** Pointer to an element stored in a templated container. */

	public:
		/********** Constructors & Destructor **********/
		
		/**
		 * Default constructor.
		*/
		array_iterator() : m_ptr(nullptr) { }

		/**
		 * Initializer constructor.
		*/
		array_iterator(pointer ptr) : m_ptr(ptr) { }

		/**
		 * Destructor.
		*/
		virtual ~array_iterator() { this->m_ptr = nullptr; }
	
	public:
		/********** Increment and Decrement Operators **********/

		/**
		 * Prefix increment operator overload.
		 * Moves the iterator forward one position.
		 * The returned value may reference an element outside the container's bounds.
		*/
		array_iterator& operator++() {
			this->m_ptr++; 	// increment pointer
			return *this; 	// return reference to this
		}

		/**
		 * Postfix increment operator overload.
		 * Moves the iterator forward one position.
		 * The returned value may reference an element outside the container's bounds.
		*/
		array_iterator operator++(int) {
			array_iterator tmp = *this; // copy old value
			++*this; 					// increment pointer
			return tmp; 				// return old value
		}

		/**
		 * Prefix decrement operator overload.
		 * Moves the iterator backwards one position.
		 * The returned value may reference an element outside the container's bounds.
		*/
		array_iterator& operator--() {
			this->m_ptr--; 	// decrement pointer
			return *this; 	// return reference to this
		}

		/**
		 * Postfix decrement operator overload.
		 * Moves the iterator backwards one position.
		 * The returned value may reference an element outside the container's bounds.
		*/
		array_iterator operator--(int) {
			array_iterator tmp = *this; // copy old value
			--*this; 					// decrement pointer
			return tmp; 				// return old value
		}
	
	public:
		/********** Compound Assignment Operators **********/

		/**
		 * Compund assignment operator overload.
		 * Moves the iterator forward n positons.
		 * The returned value may reference an element outside the container's bounds.
		*/
		array_iterator& operator+=(difference_type n){
			this->m_ptr += n; 	// move pointer forwards n steps
			return *this; 		// return reference to this
		}

		/**
		 * Compund assignment operator overload.
		 * Moves the iterator backwards n positions.
		 * The returned value may reference an element outside the container's bounds.
		*/
		array_iterator& operator-=(difference_type n) {
			this->m_ptr -= n; 	// move pointer backwards n steps
			return *this; 		// return reference to this
		}
	
	public:
		/********** Binary Arithmetic Operators **********/
		
		/**
		 * Adition operator overload.
		 * Returns an iterator that references the element n positions after the element
		 * referenced by this iterator.
		 * The returned value may reference an element outside the container's bounds.
		*/
		array_iterator operator+(difference_type n) {
			return nostl::array_iterator<_arr>(this->m_ptr + n);
		}

		/**
		 * Subtraction operator overload.
		 * Returns an iterator that references the element n positions before the 
		 * element referenced by this iterator.
		 * The returned value may reference an element outside the container's bounds.
		*/
		array_iterator operator-(difference_type n) {
			return nostl::array_iterator<_arr>(this->m_ptr - n);
		}
	
	public:
		/********** Array Subscript Operators **********/

		/**
		 * Subscript operator overload.
		 * Returns a reference to the element idx positions from the element referenced 
		 * by this iterator.
		*/
		reference operator[](size_t idx) {
			return *(this->m_ptr + idx); // return reference to the element idx positions after this
		}

		/**
		 * Subscript operator overload (const).
		 * Returns a const reference to the element idx positions from the element 
		 * referenced by this iterator.
		*/
		const reference operator[](size_t idx) const {
			return *(this->m_ptr + idx); // return const reference to the element idx positions after this
		}
	
	public:
		/********** Pointer Member Access and Dereference Operators **********/

		/**
		 * Pointer Member access operator overload.
		 * Returns a pointer to the element currently referenced by this iterator.
		*/
		pointer operator->() {
			return this->m_ptr; // return pointer
		}

		/**
		 * Pointer Member access operator overload (const).
		 * Returns a const pointer to the element currently referenced by this iterator.
		*/
		const pointer operator->() const {
			return this->m_ptr; // return const pointer
		}

		/**
		 * Dereference operator overload.
		 * Returns a reference to the element currently referenced by this iterator.
		*/
		reference operator*() {
			return *this->m_ptr; // return reference to dereferenced pointer
		}

		/**
		 * Dereference operator overload (const).
		 * Returns a const reference to the element currently referenced by this iterator.
		*/
		const reference operator*() const {
			return *this->m_ptr; // return const reference to dereferenced pointer
		}
	
	public:
		/********** Comparison Operators (Non-Member Functions) **********/

		/**
		 * Equality operator overload.
		*/
		friend inline bool operator==(const array_iterator& lhs, const array_iterator& rhs) {
			return lhs.m_ptr == rhs.m_ptr; // check if pointer are the same
		}

		/**
		 * Inequality operator overload.
		*/
		friend inline bool operator!=(const array_iterator& lhs, const array_iterator& rhs) {
			return !(lhs == rhs); // check if pointer are different
		}

		/**
		 * Less than operator overload.
		*/
		friend inline bool operator<(const array_iterator& lhs, const array_iterator& rhs) {
			return lhs.m_ptr < rhs.m_ptr; // check if left-hand side is less than right-hand side
		}		

		/**
		 * Greater than operator overload.
		*/
		friend inline bool operator>(const array_iterator& lhs, const array_iterator& rhs) {
			return rhs < lhs; // call operator< to check if right-hand side is less than left-hand side
		}

		/**
		 * Less than or equal to operator overload.
		*/
		friend inline bool operator<=(const array_iterator& lhs, const array_iterator& rhs) {
			return !(lhs > rhs); // call operator> to check if lhs is less than rhs and negate result to factor in equality
		}

		/**
		 * Greater than or equal to operator overload.
		*/
		friend inline bool operator>=(const array_iterator& lhs, const array_iterator& rhs) {
			return !(lhs < rhs); // call operator< to check if lhs is grater than rhs and negate result to factor in equality
		}

	}; // class array_iterator
	
	/**
	 * Reverse (bidirectional & random-access) iterator type for templated 
	 * containers that use an underlying C-style array for storing data contiguously
	 * in memory, such that simple pointer arithmetics can be used to interate 
	 * throught the container.
	 * References an element stored in the container.
	*/
	template<typename _arr>
	class reverse_array_iterator : public array_iterator<_arr> {

		// ...

	}; // class reverse_array_iterator

} // namespace nostl

#endif // NOSTL_ARR_ITERATORS_H

/** @todo implement reverse_iterator */
