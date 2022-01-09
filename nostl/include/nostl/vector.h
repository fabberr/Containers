#ifndef NOSTL_VECTOR_H
#define NOSTL_VECTOR_H

/********** Included Headers **********/

// C++ library
#include <vector> 			// std::vector
#include <string>			// std::string
#include <iostream> 		// std::cout, std::ostream, std::hex
#include <type_traits> 		// std::is_fundamental, std::is_pointer, std::is_member_pointer, std::is_scalar
#include <utility> 			// std::move, std::forward
#include <initializer_list> // std::initializer_list
#include <algorithm> 		// std::max
#include <functional> 		// std::function

// C library
#include <cstddef> 	// size_t, ptrdiff_t
#include <cstring> 	// std::memcpy, std::memmove, std::memset
#include <cmath> 	// std::ceil
#include <cassert> 	// assert macro

// libnostl
#include <nostl/arr_iterators.h>

/********** vector.h **********/

namespace nostl {

	/**
	 * Capacity expansion policy flags.
	*/
	enum class policy_flags {
		// two complementary states for facilitating toggling on/off
		NORMAL 		= 0x7F, /** [0111 1111] Sets the capacity expansion policy to its normal behavior. */
		RESTRICTIVE = 0x80 	/** [1000 0000] Sets restrictive capacity expansion mode. Causes the vector to always expand by 10% of its current max. capacity, regardless of its value. */
	}; // enum class policy_flags

	/** Bitwise unary NOT operator overload for enum class policy_flags */
	inline constexpr 
	policy_flags 
	operator~(
		const policy_flags& current
	) { return policy_flags(~static_cast<int>(current)); }

	/** Bitwise AND operator overload for enum class policy_flags */
	inline constexpr 
	policy_flags 
	operator&(
		const policy_flags& lhs, 
		const policy_flags& rhs
	) { return policy_flags(static_cast<int>(lhs) & static_cast<int>(rhs)); }
	
	/** Bitwise OR operator overload for enum class policy_flags */
	inline constexpr 
	policy_flags 
	operator|(
		const policy_flags& lhs, 
		const policy_flags& rhs
	) { return policy_flags(static_cast<int>(lhs) | static_cast<int>(rhs)); }
	
	/** Bitwise XOR operator overload for enum class policy_flags */
	inline constexpr 
	policy_flags 
	operator^(
		const policy_flags& lhs, 
		const policy_flags& rhs
	) { return policy_flags(static_cast<int>(lhs) ^ static_cast<int>(rhs)); }

	/** Bitwise AND assignment operator overload for enum class policy_flags */
	inline constexpr 
	policy_flags 
	operator&=(
		policy_flags& lhs, 
		const policy_flags& rhs
	) { return lhs = lhs & rhs; }

	/** Bitwise OR assignment operator overload for enum class policy_flags */
	inline constexpr 
	policy_flags 
	operator|=(
		policy_flags& lhs, 
		const policy_flags& rhs
	) { return lhs = lhs | rhs; }

	/** Bitwise XOR assignment operator overload for enum class policy_flags */
	inline constexpr 
	policy_flags 
	operator^=(
		policy_flags& lhs, 
		const policy_flags& rhs
	) { return lhs = lhs ^ rhs; }

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

		typedef T 					value_type; 		/** Type of values stored in the vector. */
		typedef size_t 				size_type; 			/** Size type. */
		typedef std::ptrdiff_t 		difference_type; 	/** Pointer difference type. */
		typedef nostl::policy_flags policy; 			/** Policy flags bitmask type */

		typedef T* 			pointer; 			/** Pointer to value type */
		typedef const T* 	const_pointer; 		/** Pointer to const value type. */
		typedef T& 			reference; 			/** Reference to value type */
		typedef const T& 	const_reference; 	/** Reference to const value type */

		typedef nostl::array_iterator<nostl::vector<T>> 		iterator; 		/** Normal iterator type. */
		typedef nostl::array_iterator<const nostl::vector<T>> 	const_iterator; /** Normal const iterator type. */

		// typedef nostl::reverse_array_iterator<iterator> 		reverse_iterator; 		/** Reverse iterator type. */
		// typedef nostl::reverse_array_iterator<const_iterator> 	const_reverse_iterator; /** Reverse const iterator type. */

	private:
		/********** Private Members **********/

		T* 		m_data;			/* Heap-allocated array for storing elements. */
		size_t 	m_size;			/* Number of elements currently on the vector. */
		size_t 	m_capacity; 	/* Maximum number of elements that can be held in the vector. */
		policy 	m_mem_policy; 	/** Current capacity expansion behavior. */

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

		constexpr T* data() noexcept;
		constexpr const T* data() const noexcept;

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

		bool plcy_normal() const;
		bool plcy_restrictive() const;
		policy policy_flags() const;
		policy policy_flags(const policy& policy);
		const policy& toggle_restrictive();

	private:
		/********** Private Member Function Declarations **********/

		inline size_t expand_to_fit() const;

	public:
		/********** Operator Overload Declarations **********/

		const T& operator[](size_t idx) const;
		T& operator[](size_t idx);

		vector& operator+=(const T& elem);
		vector& operator+=(T&& elem);

		vector& operator=(const vector& other);
		vector& operator=(const std::vector<T>& other);
		vector& operator=(vector&& other);

	}; // class vector

/********** Template Argument Deduction Guides **********/

/**
 * Vector constructed from initializer list -> deduce:
 *   > type: type of initializer list
 *   > size: size of list
*/
template<typename T, typename... U>
vector(T, U...) -> vector<T, 1 + sizeof... (U)>;

/**
 * const vector constructed from initializer list -> deduce:
 *   > type: type of initializer list
 *   > size: size of list
*/
template<typename T, typename... U>
vector(const T, U...) -> vector<T, 1 + sizeof... (U)>;

/********** Constructors & Destructor Definitions **********/

/**
 * Default constructor.
*/
template<typename T, size_t N>
nostl::vector<T, N>::vector() : 
	// initialize members
	m_data(nullptr), 
	m_size(0), 
	m_capacity(N), 
	m_mem_policy(nostl::policy_flags::NORMAL)
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
	m_capacity(N), 
	m_mem_policy(nostl::policy_flags::NORMAL)
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
	m_capacity(N), 
	m_mem_policy(nostl::policy_flags::NORMAL)
{
	// std::cout << "constructing instance from initializer list\n";
	
	// iterator type aliases
	using itr_t = typename std::initializer_list<T>::iterator;

	// Allocate initial memory for m_data.
	// Calling vector::resize with m_size set to 0 and m_data pointing to NULL only 
	// causes a new array of lenght new_capacity (ilist.size() in this case) to be 
	// allocated and set to m_data. Since there is no old data to be copied anyway, 
	// no data will be copied into this new array.
	this->resize(std::max(N, ilist.size()));

	// move elements from initializer list into this instance
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
	m_capacity(N), 
	m_mem_policy(other.m_mem_policy)
{
	// std::cout << "copy-constructing instance\n";

	// Allocate enough memory for m_data to fit contents of other vector.
	// Calling vector::resize with m_size set to 0 and m_data pointing to NULL only 
	// causes a new array of lenght new_capacity (other.m_size in this case) to be 
	// allocated and set to m_data. Since there is no old data to be copied anyway, 
	// no data will be copied into this new array, copying is done here instead.
	this->resize(std::max(N, other.m_size));

	// Check if T is a primitive and copy data accordingly.
	// For a comprehensible table of what is considered a scalar type, see: 
	// <https://www.cplusplus.com/reference/type_traits/>
	if (std::is_scalar<T>::value) {
		// T is a scalar, use std::memcpy
		if (this->m_data && other.data()) { 				// nullptr checks
			const size_t count = other.memsize(); 			// bytes
			std::memcpy(this->m_data, other.data(), count); // dst, src, byte count
		}
	} else {
		// T is not a scalar, use copy constructor
		for (size_t i = 0; i < other.len(); ++i) {
			// Copy data from other vector into this vector.
			// Use placement new for non-scalar types so that the copy constructor is actually 
			// called.
			new(&this->m_data[i]) T(other.m_data[i]);
		}
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
	m_capacity(N), 
	m_mem_policy(nostl::policy_flags::NORMAL)
{
	// std::cout << "copy-constructing instance (from std::vector)\n";

	// Allocate enough memory for m_data to fit contents of other std::vector.
	// Calling vector::resize with m_size set to 0 and m_data pointing to NULL only 
	// causes a new array of lenght new_capacity (other.size() in this case) to be 
	// allocated and set to m_data. Since there is no old data to be copied anyway, 
	// no data will be copied into this new array, copying is done here instead.
	this->resize(std::max(N, other.size()));

	// Check if T is a primitive and copy data accordingly.
	// For a comprehensible table of what is considered a scalar type, see: 
	// <https://www.cplusplus.com/reference/type_traits/>
	if (std::is_scalar<T>::value) {
		// T is a scalar, use std::memcpy
		if (this->m_data && other.data()) { 				// nullptr checks
			const size_t count = other.size() * sizeof (T); // bytes
			std::memcpy(this->m_data, other.data(), count); // dst, src, byte count
		}
	} else {
		// T is not a scalar, use copy constructor
		for (size_t i = 0; i < other.size(); ++i) {
			// Copy data from other vector into this vector.
			// Use placement new for non-scalar types so that the copy constructor is actually 
			// called.
			new(&this->m_data[i]) T(other[i]);
		}
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
	m_capacity(other.m_capacity), 
	m_mem_policy(other.m_mem_policy)
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

	// clear vector by calling the destructor of each element
	this->clear();

	// deallocate raw memory block without calling any destructors
	::operator delete(this->m_data, this->m_capacity);

	// leave this in an "empty" state
	this->m_data = nullptr;
	this->m_size = this->m_capacity = 0;
	this->m_mem_policy = nostl::policy_flags::NORMAL;
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
	for (auto& e : *this) {
		e.~T();
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
	T* new_block = static_cast<T*>(::operator new(sizeof (T) * new_capacity));

	// Update m_size if new capacity is smaller than it, save a copy of original size.
	// Causes only up to new_capacity elements to be copied if shrinking array, 
	// otherwise, copies every element.
	size_t originalSize = this->m_size;
	if (new_capacity < this->m_size) {
		this->m_size = new_capacity;
	}
	
	// Check if T is a primitive and copy data accordingly.
	// For a comprehensible table of what is considered a scalar type, see: 
	// <https://www.cplusplus.com/reference/type_traits/>
	if (std::is_scalar<T>::value) {
		// T is a scalar, use std::memcpy
		if (this->m_data) { 								// nullptr check
			const size_t count = this->m_size * sizeof (T); // bytes
			std::memcpy(new_block, this->m_data, count); 	// dst, src, byte count
		}
	} else {
		// T is not a scalar, use move constructor
		for (size_t i = 0; i < this->m_size; i++) {
			// Move each element into the new block.
			// Use placement new for non-scalar types so that the constructor is actually 
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
 * Reduces the vector's maximum capacity to be equal to its length.
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
	// Use placement new for non-scalar types so that the copy constructor is actually
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
	// Use placement new for non-scalar types so that the constructor is actually 
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
 * Returns a pointer to the underlying array used to store the data.
*/
template<typename T, size_t N>
constexpr T* nostl::vector<T, N>::data() noexcept {
	return this->m_data;
}

/**
 * Returns a const pointer to the underlying array used to store the data.
*/
template<typename T, size_t N>
constexpr const T* nostl::vector<T, N>::data() const noexcept {
	return this->m_data;
}

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

/**
 * Returns true if the expansion capacity policy is set to its normal behavior.
*/
template<typename T, size_t N>
bool nostl::vector<T, N>::plcy_normal() const {
	nostl::policy_flags mask{nostl::policy_flags::NORMAL};
	return static_cast<int>(this->m_mem_policy & mask) != 0;
}

/**
 * Returns true if the expansion capacity policy is set to restrictive mode.
*/
template<typename T, size_t N>
bool nostl::vector<T, N>::plcy_restrictive() const {
	nostl::policy_flags mask{nostl::policy_flags::RESTRICTIVE};
	return static_cast<int>(this->m_mem_policy & mask) != 0;
}

/**
 * Returns the current capacity expansion policy.
*/
template<typename T, size_t N>
nostl::policy_flags nostl::vector<T, N>::policy_flags() const {
	return this->m_mem_policy;
}

/**
 * Sets the capacity expansion policy and returns the old value.
*/
template<typename T, size_t N>
nostl::policy_flags nostl::vector<T, N>::policy_flags(const nostl::policy_flags& policy) {
	nostl::policy_flags old = this->m_mem_policy;
	this->m_mem_policy = policy;
	return old;
}

/**
 * Toggles the restrictive capacity expansion behavior on and off and returns 
 * its current value.
 * When restrictive capacity expansion mode is on, the vector will always expand
 * by 10% of its current max. capacity, regardless of its value.
*/
template<typename T, size_t N>
const nostl::policy_flags& nostl::vector<T, N>::toggle_restrictive() {
	this->m_mem_policy = ~this->m_mem_policy; 	// toggle behavior
	return this->m_mem_policy; 					// return new behavior
}

/********** Private Member Function Definitions **********/

/**
 * Helper function for vector::push_back, vector::emplace_back and vector::
 * insert. Called if the vector is at full capacity when trying to append a new
 * element. Returns an integral value to be set as the vector's new capacity.
 * 
 * If the vector is large (defined as having a current allocated capacity enough
 * to store 1000 elements or greater), this value will be 10% greater than the 
 * current capacity. For smaller vectors, this value will be 50% greater than 
 * the current capacity.
 * 
 * If restrictive capacity expansion mode is active, the value will always be 
 * 10% greater than the current capacity, regardless of its value.
*/
template<typename T, size_t N>
inline size_t nostl::vector<T, N>::expand_to_fit() const {
	
	// Determine expansion factor, compute and return new capacity.
	// The factor is determined by a combination both restrictive capacity 
	// expansion mode and current capacity checks.
	float factor = (this->plcy_restrictive() || this->m_capacity >= 1000) ? 1.1f : 1.5f;
	return static_cast<size_t>(std::ceil(this->m_capacity * factor));
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

	// set new capacity expansion policy
	this->m_mem_policy = other.m_mem_policy;

	// discard old data and resize this vector to fit contents of other vector
	this->clear();
	this->resize(std::max(N, other.m_size));

	// Check if T is a primitive and copy data accordingly.
	// For a comprehensible table of what is considered a scalar type, see: 
	// <https://www.cplusplus.com/reference/type_traits/>
	if (std::is_scalar<T>::value) {
		// T is a scalar, use std::memcpy
		if (this->m_data && other.data()) { 				// nullptr checks
			const size_t count = other.memsize(); 			// bytes
			std::memcpy(this->m_data, other.data(), count); // dst, src, byte count
		}
	} else {
		// T is not a scalar, use copy constructor
		for (size_t i = 0; i < other.len(); ++i) {
			// Copy data from other vector into this vector.
			// Use placement new for non-scalar types so that the copy constructor is actually 
			// called.
			new(&this->m_data[i]) T(other.m_data[i]);
		}
	}
	this->m_size = other.m_size; // set new size

	return *this;
}

/**
 * Copy assignment (from std::vector) operator overload.
 * Copies the data from other into this instance. Old values will be destroyed.
*/
template<typename T, size_t N>
nostl::vector<T, N>& nostl::vector<T, N>::operator=(const std::vector<T>& other) {
	// std::cout << "copy-assigning (from std::vector) into instance\n";

	// discard old data and resize this vector to fit contents of other vector
	this->clear();
	this->resize(std::max(N, other.size()));

	// Check if T is a primitive and copy data accordingly.
	// For a comprehensible table of what is considered a scalar type, see: 
	// <https://www.cplusplus.com/reference/type_traits/>
	if (std::is_scalar<T>::value) {
		// T is a scalar, use std::memcpy
		if (this->m_data && other.data()) { 				// nullptr checks
			const size_t count = other.size() * sizeof (T); // bytes
			std::memcpy(this->m_data, other.data(), count); // dst, src, byte count
		}
	} else {
		// T is not a scalar, use copy constructor
		for (size_t i = 0; i < other.size(); ++i) {
			// Copy data from other vector into this vector.
			// Use placement new for non-scalar types so that the copy constructor is actually 
			// called.
			new(&this->m_data[i]) T(other[i]);
		}
	}
	this->m_size = other.size(); // set new size

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
	this->m_mem_policy = other.m_mem_policy;

	// leave other vector in an "empty" state
	other.m_data = nullptr;
	other.m_size = other.m_capacity = 0;
	other.m_mem_policy = nostl::policy_flags::NORMAL;

	return *this;
}

/********** Free Functions **********/

/**
 * Default stream insertion operator overload for generic specialization.
*/
template<typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const nostl::vector<T, N>& rhs) {

	// iterator type alias
	using itr_t = typename nostl::vector<T, N>::const_iterator;

	// defining stream insertion operation
	std::function<void(itr_t&)> ostream_insert; // stream insertion function
	if (std::is_fundamental<T>::value) {
		// T is fundamental, simply insert
		ostream_insert = [&os](itr_t& it){ os << *it; };
	} else if (std::is_pointer<T>::value || std::is_member_pointer<T>::value) {
		// T is a pointer, insert in base16
		ostream_insert = [&os](itr_t& it){
			const auto flags = os.flags(); 	// save current formatting flags
			os << std::hex << *it; 			// insert element in base16
			os.flags(flags); 				// set flags to their original state
		};
	} else {
		// default case
		ostream_insert = [&os](itr_t& it) { os << "{ " << *it << " }"; };
	}

	// begin vector
	os << "[";

	// iterate through vector, inserting each element
	itr_t it;
	for (it = rhs.begin(); (it + 1) != rhs.end(); ++it) {
		// insert currrent element, followed by a comma
		ostream_insert(it);
		os << ", ";
	}
	if (it.get_ptr()) {
		ostream_insert(it); // insert last element
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

	// iterator type alias
	using itr_t = typename nostl::vector<std::string, N>::const_iterator;

	// begin vector
	os << "[";

	// iterate through vector, inserting each string
	itr_t it;
	for (it = rhs.begin(); (it + 1) != rhs.end(); ++it) {
		// insert currrent string, followed by a comma
		os << '\"' << *it << '\"' << ", ";
	}
	if (it.get_ptr()) {
		os << '\"' << *it << '\"'; // insert last string
	}

	// end vector
	os << "]";

	// return reference to output stream
	return os;
}

} // namespace nostl

#endif // NOSTL_VECTOR_H

/** @todo insert at arbitrary position function */
/** @todo add compare member function and comparison operator overloads */
/** @todo replace casts with static_cast */
/** @todo swap function */
/** @todo erase range function */
/** @todo improve doxygen documentation */
