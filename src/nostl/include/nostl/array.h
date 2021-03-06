#ifndef NOSTL_ARRAY
#define NOSTL_ARRAY

/********** Included Headers **********/

// C++ library
#include <initializer_list> // std::initializer_list
#include <array> 			// std::array
#include <type_traits> 		// std::is_fundamental, std::is_pointer, std::is_member_pointer, std::is_scalar
#include <iostream> 		// std::cout, std::ostream, std::hex
#include <functional> 		// std::function

// C library
#include <cstddef> 	// size_t. ptrdiff_t
#include <cstring> 	// std::memmove, std::memcpy, std::memset
#include <cassert> 	// assert macro

// libnostl
#include <nostl/arr_iterators.h>

/********** array.h **********/

namespace nostl {

	/**
	 * Fixed-length random-access container using an underlying C-style stack-
	 * allocated array for storing data contiguously in memory.
	 * 
	 * @tparam T Type of values stored in the array.
	 * @tparam N Size of array.
	*/
	template<typename T, size_t N>
	class array {
	public:
		/********** Member Types **********/

		typedef T 				value_type; 		/** Type of values stored in the array. */
		typedef size_t 			size_type; 			/** Size type. */
		typedef std::ptrdiff_t 	difference_type; 	/** Pointer difference type. */

		typedef T* 			pointer; 			/** Pointer to value type */
		typedef const T* 	const_pointer; 		/** Pointer to const value type. */
		typedef T& 			reference; 			/** Reference to value type */
		typedef const T& 	const_reference; 	/** Reference to const value type */

		typedef nostl::array_iterator<nostl::array<T, N>> 		iterator; 		/** Normal iterator type. */
		typedef nostl::array_iterator<const nostl::array<T, N>> const_iterator; /** Normal const iterator type. */

	private:
		/********** Private Members **********/
		
		T m_data[N]; 	/** Stack-allocated array for storing elements. */
	
	public:
		/********** Constructors & Destructor Declarations **********/
		
		array(const_reference value = T());
		array(const std::initializer_list<value_type>& ilist);

		array(const array& other);
		array(const std::array<value_type, N>& other);
		array(array&& other);

		/** Default destructor. */
		virtual ~array() = default;

	public:
		/********** Public Member Function Declarations **********/

		void fill(const_reference value = T());

		inline constexpr size_type len() const noexcept;

		inline constexpr bool empty() const noexcept;

		constexpr pointer data() noexcept;
		constexpr const_pointer data() const noexcept;

		reference at(size_type idx);
		const_reference at(size_type idx) const;

		const_reference front() const;
		reference front();

		const_reference back() const;
		reference back();

		iterator begin();
		const_iterator begin() const;
		const_iterator cbegin();

		iterator end();
		const_iterator end() const;
		const_iterator cend();
	
	private:
		/********** Private Member Function Declarations **********/
		
		void seek_and_destroy();

	public:
		/********** Operator Overload Declarations **********/

		const_reference operator[](size_type idx) const;
		reference operator[](size_type idx);

		array<T, N>& operator=(const array<T, N>& other);
		array<T, N>& operator=(const std::array<T, N>& other);
		array<T, N>& operator=(array<T, N>&& other);
	
	}; // class array

/********** Template Argument Deduction Guides **********/

/**
 * Array constructed from initializer list -> deduce:
 *   > type: type of initializer list
 *   > size: size of list
*/
template<typename T, typename... U>
array(T, U...) -> array<T, 1 + sizeof... (U)>;

/**
 * const array constructed from initializer list -> deduce:
 *   > type: type of initializer list
 *   > size: size of list
*/
template<typename T, typename... U>
array(const T, U...) -> array<T, 1 + sizeof... (U)>;

/********** Constructors & Destructor Definitions **********/

/**
 * @brief (Default) Constructor.
 * Constructs an array with N copies of the given value. Can act as the default 
 * constructor as it can be called with no parameters, initializes all values to
 * default-constructed objects of type T in that case.
 * 
 * @param value [in] Value to initialize the array with. Optional, defaults to a
 * default-constructed object of type T.
*/
template<typename T, size_t N>
nostl::array<T, N>::array(const T& value) {
	// fill array with value
	this->fill(value);
}

/**
 * Initializer list constructor.
*/
template<typename T, size_t N>
nostl::array<T, N>::array(const std::initializer_list<T>& ilist) {

	// assertions
	assert(ilist.size() <= this->len());

	// iterator type aliases
	using itr_t = nostl::array<T, N>::iterator;
	using inititr_t = typename std::initializer_list<T>::iterator;

	// Check if T is a primitive and copy data accordingly.
	// For a comprehensible table of what is considered a scalar type, see: 
	// <https://www.cplusplus.com/reference/type_traits/>
	if (std::is_scalar<T>::value) {
		// T is a scalar, use std::memmove and std::memset
		// move
		size_t count = ilist.size() * sizeof(T); 	// bytes
		const inititr_t src = ilist.begin(); 		// initializer_list<_E>::iterator is _E*
		if (this->m_data && src) { 					// nullptr check
			std::memmove(this->m_data, src, count); // dst, src, byte count
		}
		
		// fill
		if (ilist.size() < this->len()) {
			count = (this->len() - ilist.size()) * sizeof (T); 	// updated count
			T* dst = this->m_data + ilist.size(); 				// offset, advance pointer ilist.size() positions
			std::memset(dst, T(), count); 						// dst, value, byte count
		}
	} else {
		// T is not a scalar, use move constructor
		// move
		itr_t it = this->begin();
		inititr_t l = ilist.begin();
		while (l != ilist.end()) {
			*it++ = std::move(*l++);
		}

		// fill
		while (it != this->end()) {
			// no-op when initializer list has same size as array
			*it++ = T();
		}
	}
}

/**
 * Copy constructor.
*/
template<typename T, size_t N>
nostl::array<T, N>::array(const nostl::array<T, N>& other) {

	// iterator type aliases
	using itr_t = nostl::array<T, N>::iterator;
	using citr_t = nostl::array<T, N>::const_iterator;

	// Check if T is a primitive and copy data accordingly.
	// For a comprehensible table of what is considered a scalar type, see: 
	// <https://www.cplusplus.com/reference/type_traits/>
	if (std::is_scalar<T>::value) {
		// T is a scalar, use std::memcpy
		if (this->m_data && other.data()) {
			const size_t count = this->len() * sizeof (T); 	// bytes
			std::memcpy(this->m_data, other.data(), count); // dst, src, byte count
		}
	} else {
		// T is not a scalar, use copy constructor
		itr_t i = this->begin();
		citr_t j = other.begin();
		while (i != this->end()) {
			*i++ = *j++;
		}
	}
}

/**
 * Copy (from std::array) constructor.
*/
template<typename T, size_t N>
nostl::array<T, N>::array(const std::array<T, N>& other) {

	// iterator type aliases
	using itr_t = nostl::array<T, N>::iterator;
	using citr_t = typename std::array<T, N>::const_iterator;

	// Check if T is a primitive and copy data accordingly.
	// For a comprehensible table of what is considered a scalar type, see: 
	// <https://www.cplusplus.com/reference/type_traits/>
	if (std::is_scalar<T>::value) {
		// T is a scalar, use std::memcpy
		if (this->m_data && other.data()) {
			const size_t count = this->len() * sizeof (T); 	// bytes
			std::memcpy(this->m_data, other.data(), count); // dst, src, byte count
		}
	} else {
		// T is not a scalar, use copy constructor
		itr_t i = this->begin();
		citr_t j = other.begin();
		while (i != this->end()) {
			*i++ = *j++;
		}
	}
}

/**
 * Move constructor.
 * Transfers the ownership of the other array's members into this instance. The 
 * other array will be left in an invalid "empty" state.
*/
template<typename T, size_t N>
nostl::array<T, N>::array(nostl::array<T, N>&& other) {

	// iterator type alias
	using itr_t = nostl::array<T, N>::iterator;

	// Check if T is a primitive and move data accordingly.
	// For a comprehensible table of what is considered a scalar type, see: 
	// <https://www.cplusplus.com/reference/type_traits/>
	if (std::is_scalar<T>::value) {
		// T is a scalar, use std::memmove
		const size_t count = this->len() * sizeof (T); // bytes
		if (this->m_data && other.m_data) {
			std::memmove(this->m_data, other.data(), count); // dst, src, byte count
		}

		// leave other array in an "empty" state
		std::memset(other.m_data, 0, count); // dst, value, byte count
	} else {
		// T is not a scalar, call move constructor
		itr_t i = this->begin(), j = other.begin();
		while (i != this->end()) {
			*i++ = std::move(*j++);
		}

		// leave other array in an "empty" state
		for (auto& e : other) {
			e.~T(); // destroy object
		}
	}
}

/********** Public Member Function Definitions **********/

/**
 * Fills array with desired value.
 * 
 * @param value [in] Value to fill the array with. Optional, defaults to a 
 *        default-constructed object of type T.
*/
template<typename T, size_t N>
void nostl::array<T, N>::fill(const T& value) {

	// copy N instances of value into this array
	if (std::is_scalar<T>::value) {
		// T is a scalar, use std::memset
		std::memset(this->m_data, value, N * sizeof(T));
	} else {
		// default case, use copy constructor
		for (auto& e : *this) {
			e = value;
		}
	}
}

/**
 * Returns the number of elements that can be stored in the array.
*/
template<typename T, size_t N>
inline constexpr size_t nostl::array<T, N>::len() const noexcept { return N; }

/**
 * Checks whether the array is empty or not (N = 0).
*/
template<typename T, size_t N>
inline constexpr bool nostl::array<T, N>::empty() const noexcept { return !N; }

/**
 * Returns a pointer to the underlying array used to store the data.
*/
template<typename T, size_t N>
constexpr T* nostl::array<T, N>::data() noexcept {
	return this->m_data;
}

/**
 * Returns a const pointer to the underlying array used to store the data.
*/
template<typename T, size_t N>
constexpr const T* nostl::array<T, N>::data() const noexcept {
	return this->m_data;
}

/**
 * Member access function.
 * Returns a reference to an element in the array given its index.
*/
template<typename T, size_t N>
T& nostl::array<T, N>::at(size_t idx) {
	assert(idx < this->len());
	return (*this)[idx];
}

/**
 * const member access function.
 * Returns a const reference to an element in the array given its index.
*/
template<typename T, size_t N>
const T& nostl::array<T, N>::at(size_t idx) const {
	assert(idx < this->len());
	return (*this)[idx];
}

/**
 * Returns a const reference to the first element.
*/
template<typename T, size_t N>
const T& nostl::array<T, N>::front() const { return (*this)[0]; }

/**
 * Returns a reference to the first element.
*/
template<typename T, size_t N>
T& nostl::array<T, N>::front() { return (*this)[0]; }

/**
 * Returns a const reference to the last element.
*/
template<typename T, size_t N>
const T& nostl::array<T, N>::back() const { return (*this)[this->len() ? this->len() - 1 : 0]; }

/**
 * Returns a reference to the last element.
*/
template<typename T, size_t N>
T& nostl::array<T, N>::back() { return (*this)[this->len() ? this->len() - 1 : 0]; }

/**
 * Returns an iterator that references the address of the first element of this
 * array.
*/
template<typename T, size_t N>
typename nostl::array<T, N>::iterator nostl::array<T, N>::begin() {
	return iterator(this->m_data);
}

/**
 * Returns a const iterator that references the address of the first element of 
 * this array.
*/
template<typename T, size_t N>
typename nostl::array<T, N>::const_iterator nostl::array<T, N>::begin() const {
	return const_iterator(this->m_data);
}

/**
 * Returns a const iterator that references the address of the first element of 
 * this array.
*/
template<typename T, size_t N>
typename nostl::array<T, N>::const_iterator nostl::array<T, N>::cbegin() {
	return const_iterator(this->m_data);
}

/**
 * Returns an iterator that references the address past the last element of this
 * array.
*/
template<typename T, size_t N>
typename nostl::array<T, N>::iterator nostl::array<T, N>::end() {
	return iterator(this->m_data + N);
}

/**
 * Returns a const iterator that references the address past the last element of
 * this array.
*/
template<typename T, size_t N>
typename nostl::array<T, N>::const_iterator nostl::array<T, N>::end() const {
	return const_iterator(this->m_data + N);
}

/**
 * Returns a const iterator that references the address past the last element of
 * this array.
*/
template<typename T, size_t N>
typename nostl::array<T, N>::const_iterator nostl::array<T, N>::cend() {
	return const_iterator(this->m_data + N);
}

/********** Private Member Function Definitions **********/

/**
 * Helper function for copy and move assignment operator overloads.
 * Calls destructor of every element in the array.
*/
template<typename T, size_t N>
void nostl::array<T, N>::seek_and_destroy() {
	// searching... seek and destroy
	for (auto& e : *this) {
		e.~T();
	}
}

/********** Operator Overload Definitions **********/

/**
 * Subscript operator overload (const).
 * Returns a const reference to an element in the array given its index.
*/
template<typename T, size_t N>
const T& nostl::array<T, N>::operator[](size_type idx) const {
	assert(idx < this->len());
	return this->m_data[idx];
}

/**
 * Subscript operator overload.
 * Returns a reference to an element in the array given its index.
*/
template<typename T, size_t N>
T& nostl::array<T, N>::operator[](size_type idx) {
	assert(idx < this->len());
	return this->m_data[idx];
}

/**
 * Copy assignment operator overload.
 * Copies the data from other into this instance. Old values will be destroyed.
*/
template<typename T, size_t N>
nostl::array<T, N>& nostl::array<T, N>::operator=(const nostl::array<T, N>& other) {
	// std::cout << "copy-assigning into instance\n";

	// iterator type aliases
	using itr_t = nostl::array<T, N>::iterator;
	using citr_t = nostl::array<T, N>::const_iterator;

	// Check if T is a primitive and copy data accordingly.
	// For a comprehensible table of what is considered a scalar type, see: 
	// <https://www.cplusplus.com/reference/type_traits/>
	if (std::is_scalar<T>::value) {
		// T is a scalar, use std::memcpy
		if (this->m_data && other.data()) {
			const size_t count = this->len() * sizeof (T); 	// bytes
			std::memcpy(this->m_data, other.data(), count); // dst, src, byte count
		}
	} else {
		// T is not a scalar, use copy constructor
		// destroy old elements
		this->seek_and_destroy();

		// copy over elements from other array into this instance
		itr_t i = this->begin();
		citr_t j = other.begin();
		while (i != this->end()) {
			*i++ = *j++;
		}
	}

	return *this;
}

/**
 * Copy assignment (from std::vector) operator overload.
 * Copies the data from other into this instance. Old values will be destroyed.
*/
template<typename T, size_t N>
nostl::array<T, N>& nostl::array<T, N>::operator=(const std::array<T, N>& other) {
	// std::cout << "copy-assigning (from std::array) into instance\n";

	// iterator type aliases
	using itr_t = nostl::array<T, N>::iterator;
	using citr_t = typename std::array<T, N>::const_iterator;

	// Check if T is a primitive and copy data accordingly.
	// For a comprehensible table of what is considered a scalar type, see: 
	// <https://www.cplusplus.com/reference/type_traits/>
	if (std::is_scalar<T>::value) {
		// T is a scalar, use std::memcpy
		if (this->m_data && other.data()) {
			const size_t count = this->len() * sizeof (T); 	// bytes
			std::memcpy(this->m_data, other.data(), count); // dst, src, byte count
		}
	} else {
		// T is not a scalar, use copy constructor
		// destroy old elements
		this->seek_and_destroy();

		// copy over elements from other array into this instance
		itr_t i = this->begin();
		citr_t j = other.begin();
		while (i != this->end()) {
			*i++ = *j++;
		}
	}

	return *this;
}

/**
 * Move assignment operator overload.
 * Transfers the ownership of the other array's members into this instance. Old 
 * values will be lost
 * The other array will be left in an invalid "empty" state.
*/
template<typename T, size_t N>
nostl::array<T, N>& nostl::array<T, N>::operator=(nostl::array<T, N>&& other) {
	// std::cout << "move-assigning into instance\n";

	// iterator type alias
	using itr_t = nostl::array<T, N>::iterator;

	// Check if T is a primitive and move data accordingly.
	// For a comprehensible table of what is considered a scalar type, see: 
	// <https://www.cplusplus.com/reference/type_traits/>
	if (std::is_scalar<T>::value) {
		// T is a scalar, use std::memmove
		const size_t count = this->len() * sizeof (T); // bytes
		if (this->m_data && other.m_data) {
			std::memmove(this->m_data, other.data(), count); // dst, src, byte count
		}

		// leave other array in an "empty" state
		std::memset(other.m_data, 0, count); // dst, value, byte count
	} else {
		// T is not a scalar, call move constructor
		// destroy old elements
		this->seek_and_destroy();

		// move over elements from other array into this instance
		itr_t i = this->begin(), j = other.begin();
		while (i != this->end()) {
			*i++ = std::move(*j++);
		}

		// leave other array in an "empty" state
		for (auto& e : other) {
			e.~T(); // destroy object
		}
	}

	return *this;
}

/********** Free Functions **********/

/**
 * Default stream insertion operator overload for generic specialization.
*/
template<typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const nostl::array<T, N>& rhs) {

	// begin array
	os << "[";

	// defining stream insertion operation
	std::function<void(size_t)> ostream_insert; // stream insertion function
	if (std::is_fundamental<T>::value) {
		// T is fundamental, simply insert
		ostream_insert = [&os, &rhs](size_t i){ os << rhs[i]; };
	} else if (std::is_pointer<T>::value || std::is_member_pointer<T>::value) {
		// T is a pointer, insert in base16
		ostream_insert = [&os, &rhs](size_t i){
			const auto flags = os.flags(); 	// save current formatting flags
			os << std::hex << rhs[i]; 		// insert element in base16
			os.flags(flags); 				// set flags to their original state
		};
	} else {
		// default case
		ostream_insert = [&os, &rhs](size_t i) { os << "{ " << rhs[i] << " }"; };
	}

	// iterate through array, inserting each element
	for (size_t i = 0; (i + 1) < rhs.len(); i++) {
		// insert currrent element, followed by a comma
		ostream_insert(i);
		os << ", ";
	}
	ostream_insert(rhs.len() - 1); // insert last element

	// end array
	os << "]";

	// return reference to output stream
	return os;
}

/**
 * Stream insertion operator overload for std::string specialization.
*/
template<size_t N>
std::ostream& operator<<(std::ostream& os, const nostl::array<std::string, N>& rhs) {

	// begin array
	os << "[";

	// insert each string
	for (size_t i = 0; i < N; i++) {

		// insert string
		os << '"' << rhs[i] << '"';

		// if there are still strings to insert, insert a comma
		if (i + 1 < N) {
			os << ", ";
		}
	}

	// end array
	os << "]";

	// return reference to output stream
	return os;
}

} // namespace nostl

#endif // NOSTL_ARRAY

/** @todo add compare member function and comparison operator overloads */
/** @todo replace casts with static_cast */
/** @todo swap function */
/** @todo improve doxygen documentation */
