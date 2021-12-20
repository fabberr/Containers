#ifndef NOSTL_ARRAY
#define NOSTL_ARRAY

/********** Included Headers **********/

// C++ library
#include <initializer_list> // std::initializer_list
#include <array> 			// std::array
#include <type_traits> 		// std::is_arithmetic

// C library
#include <cstddef> // size_t. ptrdiff_t
#include <cstring> // std::memmove, std::memcpy

// libnostl
#include <nostl/arr_iterators.h>

/********** array.h **********/

namespace nostl
{

	/**
	 * Fixed-length random-access container using an underlying C-style stack-
	 * allocated array for storing data contiguously in memory.
	*/
	template<typename T, size_t N>
	class array {
	public:
		/********** Member Types **********/

		typedef T 				value_type; 		/** Type of values stored in the array. */
		typedef size_t 			size_type; 			/** Size type. */
		typedef std::ptrdiff_t 	difference_type; 	/** Pointer difference type (for pointer arithmetics in any address space). */
		typedef T* 				pointer; 			/** Pointer to value type */
		typedef const T* 		const_pointer; 		/** Pointer to const value type. */
		typedef T& 				reference; 			/** Reference to value type */
		typedef const T& 		const_reference; 	/** Reference to const value type */

		typedef nostl::array_iterator<nostl::array<T, N>> 		iterator; 		/** Normal iterator type. */
		typedef nostl::array_iterator<nostl::array<const T, N>> const_iterator; /** Normal const iterator type. */

	private:
		/********** Private Members **********/
		
		value_type m_data[N]; 	/** Stack-allocated array for storing elements. */
	
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

	public:
		/********** Operator Overload Declarations **********/

		const_reference operator[](size_type idx) const;
		reference operator[](size_type idx);

		array<T, N>& operator=(const array<T, N>& other);
		array<T, N>& operator=(array<T, N>&& other);

	public:
		/********** Friend Functions **********/

		/**
		 * Default stream insertion operator overload for any specialization.
		*/
		friend std::ostream& operator<<(std::ostream& os, const nostl::array<T, N>& rhs) {

			// begin array
			os << "[";

			// print each element
			for (size_t i = 0; i < N; i++) {

				// check if T is of a primitive type
				if (std::is_arithmetic<T>::value) {
					// print element
					os << rhs.m_data[i];
				} else {
					// print element surrounded by brackets
					os << "{ " << rhs.m_data[i] << " }";
				}

				// if there sre still elements to print, print a comma
				if (i + 1 < N) {
					os << ", ";
				}
			}

			// end array
			os << "]";

			// return reference to output stream
			return os;
		}
		
		// /**
		//  * Stream insertion operator overload for std::string specialization.
		// */
		// template<size_t fN>
		// friend std::ostream& operator<<(std::ostream& os, const nostl::array<std::string, fN>& rhs) {

		// 	// begin array
		// 	os << "[";

		// 	// print each string
		// 	for (size_t i = 0; i < fN; i++) {

		// 		// print string
		// 		os << '"' << rhs.m_data[i] << '"';

		// 		// if there sre still strings to print, print a comma
		// 		if (i + 1 < fN) {
		// 			os << ", ";
		// 		}
		// 	}

		// 	// end array
		// 	os << "]";

		// 	// return reference to output stream
		// 	return os;
		// }
	
	}; // class array

} // namespace nostl

/********** Constructors & Destructor Implementations **********/

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

	// copy N instances of value into this array
	for (auto& e : *this) {
		e = value;
	}
}

/**
 * Initializer list constructor.
*/
template<typename T, size_t N>
nostl::array<T, N>::array(const std::initializer_list<T>& ilist) {

	// move elements from initializer list into this instance
	size_t i = 0;
	for (const auto& value : ilist) {
		this->m_data[i++] = std::move(value);
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
	// For a comprehensible table of what is considered an arithmetic type, see: 
	// <https://www.cplusplus.com/reference/type_traits/is_arithmetic/>
	if (std::is_arithmetic<T>::value) {
		// T is of an arithmetic type, use std::memcpy
		std::memcpy(this->m_data, other.m_data, N * sizeof (T));
	} else {
		// T is not of an arithmetic type, call copy constructor for each element
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
	// For a comprehensible table of what is considered an arithmetic type, see: 
	// <https://www.cplusplus.com/reference/type_traits/is_arithmetic/>
	if (std::is_arithmetic<T>::value) {
		// T is of an arithmetic type, use std::memcpy
		std::memcpy(this->m_data, other.data(), N * sizeof (T));
	} else {
		// T is not of an arithmetic type, call copy constructor for each element
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
	// For a comprehensible table of what is considered an arithmetic type, see: 
	// <https://www.cplusplus.com/reference/type_traits/is_arithmetic/>
	if (std::is_arithmetic<T>::value) {
		// T is of an arithmetic type, use std::memmove
		size_t count = N * sizeof (T);
		std::memmove(this->m_data, other.m_data, count);

		// leave other array in an "empty" state
		std::memset(other.m_data, 0, count);
	} else {
		// T is not of an arithmetic type, call move constructor for each element
		itr_t i = this->begin(), j = other.begin();
		while (i != this->end()) {
			*i++ = std::move(*j++);
		}

		// leave other array in an "empty" state
		for (auto& e : other) {
			e.~T();
		}
	}
}

/********** Public Member Function Implementations **********/

/**
 * Returns the number of elements that can be stored in the array.
*/
template<typename T, size_t N>
inline constexpr size_t nostl::array<T, N>::len() const noexcept { return N; }

/**
 * Returns whether the array is empty or not (N = 0).
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

/********** Operator Overload Implementations **********/

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
	std::cout << "copy-assigning into instance\n";

	// iterator type aliases
	using itr_t = nostl::array<T, N>::iterator;
	using citr_t = nostl::array<T, N>::const_iterator;

	// Check if T is a primitive and copy data accordingly.
	// For a comprehensible table of what is considered an arithmetic type, see: 
	// <https://www.cplusplus.com/reference/type_traits/is_arithmetic/>
	if (std::is_arithmetic<T>::value) {
		// T is of an arithmetic type, use std::memcpy
		std::memcpy(this->m_data, other.m_data, N * sizeof (T));
	} else {
		// T is not of an arithmetic type, call copy constructor for each element
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
	std::cout << "move-assigning into instance\n";

	// iterator type alias
	using itr_t = nostl::array<T, N>::iterator;

	// Check if T is a primitive and move data accordingly.
	// For a comprehensible table of what is considered an arithmetic type, see: 
	// <https://www.cplusplus.com/reference/type_traits/is_arithmetic/>
	if (std::is_arithmetic<T>::value) {
		// T is of an arithmetic type, use std::memmove
		size_t count = N * sizeof (T);
		std::memmove(this->m_data, other.m_data, count);

		// leave other array in an "empty" state
		std::memset(other.m_data, 0, count);
	} else {
		// T is not of an arithmetic type, call move constructor for each element
		itr_t i = this->begin(), j = other.begin();
		while (i != this->end()) {
			*i++ = std::move(*j++);
		}

		// leave other array in an "empty" state
		for (auto& e : other) {
			e.~T();
		}
	}

	return *this;
}

#endif // NOSTL_ARRAY

/** @todo fill member function */
/** @todo swap member function */
