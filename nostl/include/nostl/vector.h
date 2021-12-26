#ifndef NOSTL_VECTOR_H
#define NOSTL_VECTOR_H

/********** Included Headers **********/

// C++ library
#include <vector> 			// std::vector
#include <string>			// std::string
#include <iostream> 		// std::cout, std::ostream, std::hex, std::dec
#include <type_traits> 		// std::is_fundamental
#include <utility> 			// std::move, std::forward
#include <initializer_list> // std::initializer_list
#include <algorithm> 		// std::max

// C library
#include <cstddef> 	// size_t, ptrdiff_t
#include <cstring> 	// std::memcpy
#include <cmath> 	// std::ceil
#include <cassert> 	// assert macro

// libnostl
#include <nostl/arr_iterators.h>

/********** vector.h **********/

namespace nostl {

	/**
	 * Dynamic random-access container using an underlying C-style heap-allocated 
	 * array for storing data contiguously in memory.
	 * 
	 * @tparam T Type of values stored in the vector.
	 * @tparam N Initial capacity of vector. Optional, defaults to 2.
	*/
	template<typename T, size_t N = 2>
	class vector {
	public:
		/********** Member Types **********/

		typedef T 				value_type; 		/** Type of values stored in the vector. */
		typedef size_t 			size_type; 			/** Size type. */
		typedef std::ptrdiff_t 	difference_type; 	/** Pointer difference type. */
		typedef T* 				pointer; 			/** Pointer to value type */
		typedef const T* 		const_pointer; 		/** Pointer to const value type. */
		typedef T& 				reference; 			/** Reference to value type */
		typedef const T& 		const_reference; 	/** Reference to const value type */

		typedef nostl::array_iterator<nostl::vector<T>> 		iterator; 		/** Normal iterator type. */
		typedef nostl::array_iterator<const nostl::vector<T>> 	const_iterator; /** Normal const iterator type. */

		// typedef nostl::reverse_array_iterator<iterator> 		reverse_iterator; 		/** Reverse iterator type. */
		// typedef nostl::reverse_array_iterator<const_iterator> 	const_reverse_iterator; /** Reverse const iterator type. */

	private:
		/********** Private Members **********/

		T* 		m_data;		/* Heap-allocated array for storing elements. */
		size_t 	m_size;		/* Number of elements currently on the vector. */
		size_t 	m_capacity; /* Maximum number of elements that can be held in the vector. */

	public:
		/********** Constructors & Destructor Declarations **********/

		vector();

		vector(size_t count, const T& value = T());
		vector(const std::initializer_list<T>& ilist);

		vector(const vector& other);
		vector(const std::vector<T>& other);

		vector(vector<T, N>&& other);

		virtual ~vector();

	public:
		/********** Public Member Function Declarations **********/

		void clear();
		void resize(size_t new_capacity);
		void shrink_to_fit();

		vector& push_back(const T& elem);
		vector& push_back(T&& elem);

		template<typename... Args>
		vector& emplace_back(Args&&... args);

		void pop_back();

		void erase(size_t idx);

		size_t len() const;
		size_t memsize() const;

		size_t capacity() const;
		size_t allocsize() const;

		bool empty() const;

		T& at(size_t idx);
		const T& at(size_t idx) const;

		const T& front() const;
		T& front();

		const T& back() const;
		T& back();

		iterator begin();
		const_iterator begin() const;
		const_iterator cbegin() const;
		
		iterator end();
		const_iterator end() const;
		const_iterator cend() const;

	public:
		/********** Operator Overload Declarations **********/

		const T& operator[](size_t idx) const;
		T& operator[](size_t idx);

		vector& operator+=(const T& elem);
		vector& operator+=(T&& elem);

		vector& operator=(const vector& other);
		vector& operator=(vector&& other);

	private:
		/********** Private Member Function Declarations **********/

		inline size_t expand_to_fit() const;

	}; // class vector

/********** Template Argument Deduction Guides **********/

/**
 * Vector constructed from initializer list -> deduce:
 *   > type: type of initializer list
 *   > size: size of list
*/
template<typename T, typename... U>
vector(T, U...) -> vector<T, sizeof... (U)>;

/**
 * const vector constructed from initializer list -> deduce:
 *   > type: type of initializer list
 *   > size: size of list
*/
template<typename T, typename... U>
vector(const T, U...) -> vector<T, sizeof... (U)>;

/********** Constructors & Destructor Definitions **********/

/**
 * Default constructor.
*/
template<typename T, size_t N>
nostl::vector<T, N>::vector() : 
	// initialize members
	m_data(nullptr), 
	m_size(0), 
	m_capacity(N)
{
	// std::cout << "constructing instance\n";

	// Allocate initial memory for m_data.
	// Calling vector::resize with m_size set to 0 and m_data pointing to NULL only 
	// causes a new array of lenght new_capacity (N in this case) to be allocated 
	// and set to m_data. Since there is no old data to be copied anyway, no data 
	// will be copied into this new array.
	this->resize(std::max(N, (size_t)1));
}

/**
 * @brief Constructor.
 * Constructs a vector with n copies of the given value.
 * 
 * @param count [in] Number of elements. If count is bigger than template 
 *        parameter N, the initial capacity will be set to count instead.
 * @param value [in] Value to initialize the vector with. Optional, defaults to 
 *        a default-constructed object of type T.
*/
template<typename T, size_t N>
nostl::vector<T, N>::vector(size_t count, const T& value) : 
	// initialize members
	m_data(nullptr), 
	m_size(0), 
	m_capacity(N)
{
	// std::cout << "constructing instance with " << count << " copies of " << value << '\n';
	
	// Allocate initial memory for m_data.
	// Calling vector::resize with m_size set to 0 and m_data pointing to NULL only 
	// causes a new array of lenght new_capacity (count or N in this case) to be 
	// allocated and set to m_data. Since there is no old data to be copied anyway, 
	// no data will be copied into this new array.
	this->resize(std::max(N, count));

	// append count instances of value into this vector
	for (size_t i = 0; i < count; i++) {
		this->emplace_back(value);
	}
}

/**
 * Initializer list constructor.
*/
template<typename T, size_t N>
nostl::vector<T, N>::vector(const std::initializer_list<T>& ilist) : 
	// initialize members
	m_data(nullptr), 
	m_size(0), 
	m_capacity(N)
{
	// std::cout << "constructing instance from initializer list\n";
	
	// Allocate initial memory for m_data.
	// Calling vector::resize with m_size set to 0 and m_data pointing to NULL only 
	// causes a new array of lenght new_capacity (ilist.size() in this case) to be 
	// allocated and set to m_data. Since there is no old data to be copied anyway, 
	// no data will be copied into this new array.
	this->resize(std::max(N, ilist.size()));

	// move elements from initializer list into this instance
	using itr_t = typename std::initializer_list<T>::iterator;
	for (itr_t it = ilist.begin(); it != ilist.end(); it++) {
		this->emplace_back(std::move(*it));
	}
}

/**
 * Copy constructor.
*/
template<typename T, size_t N>
nostl::vector<T, N>::vector(const nostl::vector<T, N>& other) : 
	// initialize members
	m_data(nullptr), 
	m_size(0), 
	m_capacity(N)
{
	// std::cout << "copy-constructing instance\n";

	// Allocate enough memory for m_data to fit contents of other vector.
	// Calling vector::resize with m_size set to 0 and m_data pointing to NULL only 
	// causes a new array of lenght new_capacity (other.m_size in this case) to be 
	// allocated and set to m_data. Since there is no old data to be copied anyway, 
	// no data will be copied into this new array, copying is done here instead.
	this->resize(std::max(N, other.m_size));

	// copy each element of other vector into this instance
	for (size_t i = 0; i < other.m_size; i++) {
		// Copy data from other vector into this vector.
		// Use placement new for complex types so that the copy constructor is actually
		// called.
		new(&this->m_data[i]) T(other.m_data[i]);
	}
	this->m_size = other.m_size; // set new size
}

/**
 * Copy (from std::vector) constructor.
*/
template<typename T, size_t N>
nostl::vector<T, N>::vector(const std::vector<T>& other) : 
	// initialize members
	m_data(nullptr), 
	m_size(0), 
	m_capacity(N)
{
	// std::cout << "copy-constructing instance (from std::vector)\n";

	// Allocate enough memory for m_data to fit contents of other std::vector.
	// Calling vector::resize with m_size set to 0 and m_data pointing to NULL only 
	// causes a new array of lenght new_capacity (other.size() in this case) to be 
	// allocated and set to m_data. Since there is no old data to be copied anyway, 
	// no data will be copied into this new array, copying is done here instead.
	this->resize(std::max(N, other.size()));

	// copy each element of other std::vector into this instance
	for (size_t i = 0; i < other.size(); i++) {
		// Copy data from other std::vector into this vector.
		// Use placement new for complex types so that the copy constructor is actually
		// called.
		new(&this->m_data[i]) T(other[i]);
	}
	this->m_size = other.size(); // set new size
}

/**
 * Move constructor.
 * Transfers the ownership of the other vector's members into this instance. The 
 * other vector will be left in an invalid "empty" state.
*/
template<typename T, size_t N>
nostl::vector<T, N>::vector(nostl::vector<T, N>&& other) : 
	// initialize members by transfering ownership of other vector's members into this instance
	m_data(other.m_data), 
	m_size(other.m_size), 
	m_capacity(other.m_capacity)
{
	// std::cout << "move-constructing instance\n";

	// leave other vector in an "empty" state
	other.m_data = nullptr;
	other.m_size = other.m_capacity = 0;
}

/**
 * Destructor.
*/
template<typename T, size_t N>
nostl::vector<T, N>::~vector() {
	// std::cout << "destroying instance\n";

	// clear vector by called the destructor for each element
	this->clear();

	// deallocate raw memory block without calling any destructors
	::operator delete(this->m_data, this->m_capacity);

	// leave this in an "empty" state, unecessary cleanup of resources about to be returned to the system but w/e
	this->m_data = nullptr;
	this->m_size = this->m_capacity = 0;
}

/********** Public Member Function Definitions **********/

/**
 * Removes all elements from the vector, destroying them. Capacity is unchanged.
 * If the elements are raw pointers, this call may cause a memory leak. 
 * Therefore, the responsibility of managing the element's memory is relegated 
 * to the caller.
*/
template<typename T, size_t N>
void nostl::vector<T, N>::clear() {

	// call destructor of each element
	for (size_t i = 0; i < this->m_size; i++) {
		this->m_data[i].~T();
	}
	this->m_size = 0; // set size to 0
}

/**
 * Changes the maximum capacity of the vector.
*/
template<typename T, size_t N>
void nostl::vector<T, N>::resize(size_t new_capacity) {

	// Allocate raw memory block (we back to malloc bois)
	// By calling ::operator new(), no construtors will be called.
	T* new_block = (T*)::operator new(sizeof (T) * new_capacity);

	// Update m_size if new capacity is smaller than it, save a copy of original size.
	// Causes only up to new_capacity elements to be copied if shrinking array, 
	// otherwise, copies every element.
	size_t originalSize = this->m_size;
	if (new_capacity < this->m_size) {
		this->m_size = new_capacity;
	}
	
	// Check if T is a primitive and copy data accordingly.
	// For a comprehensible table of what is considered a fundamental type, see: 
	// <https://www.cplusplus.com/reference/type_traits/is_fundamental/>
	if (std::is_fundamental<T>::value) {
		// T is of an fundamental type, use std::memcpy
		if (this->m_data) {
			std::memcpy(new_block, this->m_data, sizeof (T) * this->m_size);
		}
	} else {
		// T is not of an fundamental type, call move constructor for each element
		for (size_t i = 0; i < this->m_size; i++) {
			// Move each element into the new block.
			// Use placement new for complex types so that the constructor is actually 
			// called and cast m_data[i] into an r-value to attempt a call to the move 
			// constructor if it exists.
			new(&new_block[i]) T(std::move(this->m_data[i]));
		}
	}

	// clear old contents by calling the destructor of each old element
	for (size_t i = 0; i < originalSize; i++) {
		this->m_data[i].~T();
	}

	// Deallocate old raw memory block.
	// No destructors will be called, old objects were already destroyed.
	::operator delete(this->m_data, this->m_capacity);

	this->m_data = new_block; 			// assign m_data pointer to new block
	this->m_capacity = new_capacity; 	// set new capacity
}

/**
 * Reduces the vector's maximum capacity to be equal to vector's length.
*/
template<typename T, size_t N>
void nostl::vector<T, N>::shrink_to_fit() {
	this->resize(this->m_size);
}

/**
 * Appends an element to the end of the vector.
*/
template<typename T, size_t N>
nostl::vector<T, N>& nostl::vector<T, N>::push_back(const T& elem) {
	// std::cout << "copying new element into instance\n";

	// resize vector to fit new element if it's at capacity
	if (this->m_size == this->m_capacity) {
		this->resize(this->expand_to_fit());
	}

	// Copy new element into last position, update size.
	// Use placement new for complex types so that the copy constructor is actually
	// called.
	new(&this->m_data[this->m_size++]) T(elem);

	return *this;
}

/**
 * Appends an element to the end of the vector.
 * The element will be moved into place instead of copied.
*/
template<typename T, size_t N>
nostl::vector<T, N>& nostl::vector<T, N>::push_back(T&& elem) {
	// std::cout << "moving new element into instance\n";

	// resize vector to fit new element if it's at capacity
	if (this->m_size == this->m_capacity) {
		this->resize(this->expand_to_fit());
	}

	// Move new element into last position, update size.
	// Use placement new for complex types so that the constructor is actually 
	// called and cast elem into an r-value to attempt a call to the move 
	// constructor if it exists.
	new(&this->m_data[this->m_size++]) T(std::move(elem));

	return *this;
}

/**
 * Appends an element to the end of the vector.
 * The element will be constructed directly in place in the vector, no move or 
 * copy operation takes place.
*/
template<typename T, size_t N>
template<typename... Args>
nostl::vector<T, N>& nostl::vector<T, N>::emplace_back(Args&&... args) {

	// resize vector to fit new element if it's at capacity
	if (this->m_size == this->m_capacity) {
		this->resize(this->expand_to_fit());
	}

	// Construct object in-place using placement new operator.
	// Forward this call's args to new element's constructor and instantiate it in-
	// place at the memory address of the vector's last position, update size.
	new(&this->m_data[this->m_size++]) T(std::forward<Args>(args)...);

	return *this;
}

/**
 * Removes the element at the last position, destroying it.
*/
template<typename T, size_t N>
void nostl::vector<T, N>::pop_back() {

	// call destructor of last element if the vector isn't empty and update size
	if (this->m_size) {
		this->m_data[--this->m_size].~T();
	}
}

/**
 * Removes the element at the given position.
*/
template<typename T, size_t N>
void nostl::vector<T, N>::erase(size_t idx) {

	// assertions
	assert(idx < this->m_size);

	// destroy element at given idx
	this->m_data[idx].~T();

	// rearranje array
	for (size_t i = idx; i < this->m_size - 1; i++) {
		this->m_data[i] = std::move(this->m_data[i + 1]);
	}

	// destroy last element
	this->pop_back();
}

/**
 * Returns the number of elements currently stored in the vector.
*/
template<typename T, size_t N>
size_t nostl::vector<T, N>::len() const { return this->m_size; }

/**
 * Returns the total amount of memory currently used by the data stored in the 
 * vector in bytes.
*/
template<typename T, size_t N>
size_t nostl::vector<T, N>::memsize() const { return sizeof (T) * this->m_size; }

/**
 * Returns the maximum number of elements that can currently be stored in the 
 * vector.
*/
template<typename T, size_t N>
size_t nostl::vector<T, N>::capacity() const { return this->m_capacity; }

/**
 * Returns the total amount memory allocated by the vector's underlying array 
 * in bytes.
*/
template<typename T, size_t N>
size_t nostl::vector<T, N>::allocsize() const { return sizeof (T) * this->m_capacity; }

/**
 * Checks whether this vector is empty or not.
*/
template<typename T, size_t N>
bool nostl::vector<T, N>::empty() const { return this->m_size == 0; }

/**
 * Member access function.
 * Returns a reference to an element in the vector given its index.
*/
template<typename T, size_t N>
T& nostl::vector<T, N>::at(size_t idx) {
	assert(idx < this->m_size);
	return (*this)[idx];
}

/**
 * const member access function.
 * Returns a const reference to an element in the vector given its index.
*/
template<typename T, size_t N>
const T& nostl::vector<T, N>::at(size_t idx) const {
	assert(idx < this->m_size);
	return (*this)[idx];
}

/**
 * Returns a const reference to the first element.
*/
template<typename T, size_t N>
const T& nostl::vector<T, N>::front() const { return (*this)[0]; }

/**
 * Returns a reference to the first element.
*/
template<typename T, size_t N>
T& nostl::vector<T, N>::front() { return (*this)[0]; }

/**
 * Returns a const reference to the last element.
*/
template<typename T, size_t N>
const T& nostl::vector<T, N>::back() const { return (*this)[this->m_size ? this->m_size - 1 : 0]; }

/**
 * Returns a reference to the last element.
*/
template<typename T, size_t N>
T& nostl::vector<T, N>::back() { return (*this)[this->m_size ? this->m_size - 1 : 0]; }

/**
 * Returns an iterator that references the address of the first element of this
 * vector.
*/
template<typename T, size_t N>
typename nostl::vector<T, N>::iterator nostl::vector<T, N>::begin() {
	return iterator(this->m_data); // address of first element
}

/**
 * Returns a const iterator that references the address of the first element of 
 * this vector.
*/
template<typename T, size_t N>
typename nostl::vector<T, N>::const_iterator nostl::vector<T, N>::begin() const {
	return const_iterator(this->m_data); // address of first element
}

/**
 * Returns a const iterator that references the address of the first element of 
 * this vector.
*/
template<typename T, size_t N>
typename nostl::vector<T, N>::const_iterator nostl::vector<T, N>::cbegin() const {
	return const_iterator(this->m_data); // address of first element
}

/**
 * Returns an iterator that references the address past the last element of this
 * vector.
*/
template<typename T, size_t N>
typename nostl::vector<T, N>::iterator nostl::vector<T, N>::end() {
	return iterator(this->m_data + this->m_size); // address one step after last element
}

/**
 * Returns a const iterator that references the address past the last element of
 * this vector.
*/
template<typename T, size_t N>
typename nostl::vector<T, N>::const_iterator nostl::vector<T, N>::end() const {
	return const_iterator(this->m_data + this->m_size); // address one step after last element
}

/**
 * Returns a const iterator that references the address past the last element of
 * this vector.
*/
template<typename T, size_t N>
typename nostl::vector<T, N>::const_iterator nostl::vector<T, N>::cend() const {
	return const_iterator(this->m_data + this->m_size); // address one step after last element
}

/********** Operator Overload Definitions **********/

/**
 * Subscript operator overload (const).
 * Returns a const reference to an element in the vector given its index.
*/
template<typename T, size_t N>
const T& nostl::vector<T, N>::operator[](size_t idx) const {
	assert(idx < this->m_size);
	return this->m_data[idx];
}

/**
 * Subscript operator overload.
 * Returns a reference to an element in the vector given its index.
*/
template<typename T, size_t N>
T& nostl::vector<T, N>::operator[](size_t idx) {
	assert(idx < this->m_size);
	return this->m_data[idx];
}

/**
 * Compound assignment operator overload.
 * Appends an element to the end of the vector.
*/
template<typename T, size_t N>
nostl::vector<T, N>& nostl::vector<T, N>::operator+=(const T& elem) {
	return this->push_back(elem);
}

/**
 * Compound assignment operator overload.
 * Appends an element to the end of the vector.
 * The element will be moved instead of copied.
*/
template<typename T, size_t N>
nostl::vector<T, N>& nostl::vector<T, N>::operator+=(T&& elem) {
	return this->push_back(std::move(elem));
}

/**
 * Copy assignment operator overload.
 * Copies the data from other into this instance. Old values will be destroyed.
*/
template<typename T, size_t N>
nostl::vector<T, N>& nostl::vector<T, N>::operator=(const nostl::vector<T, N>& other) {
	// std::cout << "copy-assigning into instance\n";

	// discard old data and resize this vector to fit contents of other vector
	this->clear();
	this->resize(std::max(N, other.m_size));

	// copy each element of other vector into this instance
	for (size_t i = 0; i < other.m_size; i++) {
		// Copy data from other vector into this vector.
		// Use placement new for complex types so that the copy constructor is actually
		// called.
		new(&this->m_data[i]) T(other.m_data[i]);
	}
	this->m_size = other.m_size; // set new size

	return *this;
}

/**
 * Move assignment operator overload.
 * Transfers the ownership of the other vector's members into this instance. Old
 * values will be destroyed.
 * The other vector will be left in an invalid "empty" state.
*/
template<typename T, size_t N>
nostl::vector<T, N>& nostl::vector<T, N>::operator=(nostl::vector<T, N>&& other) {
	// std::cout << "move-assigning into instance\n";
	
	// discard old data and resize this vector to fit contents of other vector
	this->clear();
	this->resize(std::max(N, other.m_size));

	// transfer ownership of other vector's members into this instance
	this->m_data = other.m_data;
	this->m_size = other.m_size;
	this->m_capacity = other.m_capacity;

	// leave other vector in an "empty" state
	other.m_data = nullptr;
	other.m_size = other.m_capacity = 0;

	return *this;
}

/********** Private Member Function Definitions **********/

/**
 * Helper function for vector::append. Called if the vector is at full capacity 
 * when trying to append a new element.
 * Returns an integral value to be set as the vector's new capacity.
 * If the vector is large (defined as having a current allocated capacity enough
 * to store 1000 elements or greater), this value will be 10% greater than the 
 * current capacity.
 * For smaller vectors, the value will be 50% greater than the current capacity.
*/
template<typename T, size_t N>
inline size_t nostl::vector<T, N>::expand_to_fit() const {
	// figure out factor based on vector's current size and return new capacity
	float factor = this->m_capacity < 1000 ? 1.5f : 1.1f;
	return (size_t)(std::ceil(this->m_capacity * factor));
}

/********** Free Functions **********/

/**
 * Default stream insertion operator overload for any specialization.
*/
template<typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const nostl::vector<T, N>& rhs) {

	// begin vector
	os << "[";

	// insert each element
	for (size_t i = 0; i < rhs.len(); i++) {

		// check if T is of a primitive type
		if (std::is_fundamental<T>::value) {
			// insert element
			os << rhs[i];
		} else {
			// insert element surrounded by brackets
			os << "{ " << rhs[i] << " }";
		}

		// if there are still elements to insert, insert a comma
		if (i + 1 < rhs.len()) {
			os << ", ";
		}
	}

	// end vector
	os << "]";

	// return reference to output stream
	return os;
}

/**
 * Stream insertion operator overload for std::string specialization.
*/
template<size_t N>
std::ostream& operator<<(std::ostream& os, const nostl::vector<std::string, N>& rhs) {

	// begin vector
	os << "[";

	// insert each string
	for (size_t i = 0; i < rhs.len(); i++) {

		// insert string
		os << '"' << rhs[i] << '"';

		// if there are still strings to insert, insert a comma
		if (i + 1 < rhs.len()) {
			os << ", ";
		}
	}

	// end vector
	os << "]";

	// return reference to output stream
	return os;
}

} // namespace nostl

#endif // NOSTL_VECTOR_H

/** @todo retrictive capacity expansion mode: always expand by a factor of 1.1 when set */
/** @todo insert at arbitrary position function */
/** @todo erase range function */
/** @todo construct from iterator range */
/** @todo template argument deduction guide for iterator range constructor */
/** @todo swap() function */
