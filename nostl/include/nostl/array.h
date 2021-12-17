#ifndef NOSTL_ARRAY
#define NOSTL_ARRAY

/********** Included Headers **********/

// C++ library
#include <initializer_list> // std::initializer_list
#include <array> 			// std::array
#include <type_traits> 		// std::is_arithmetic

// C library
#include <cstddef> // size_t. ptrdiff_t

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

		typedef T 				value_type; 		/** Type of values stored in the vector. */
		typedef size_t 			size_type; 			/** Size type. */
		typedef std::ptrdiff_t 	difference_type; 	/** Pointer difference type (for pointer arithmetics in any address space). */

		typedef T* 			pointer; 		/** Pointer to value type */
		typedef const T* 	const_pointer; 	/** Pointer to const value type. */

		typedef T& 			reference; 			/** Reference to value type */
		typedef const T& 	const_reference; 	/** Reference to const value type */

		typedef nostl::array_iterator<nostl::array<T, N>> 		iterator; 		/** Normal iterator type. */
		typedef nostl::array_iterator<nostl::array<const T, N>> const_iterator; /** Normal const iterator type. */

	private:
		/********** Private Members **********/
		
		value_type m_data; /** Stack-allocated array for storing elements. */
	
	public:
		/********** Constructors & Destructor Declarations **********/
		
		/** Default default constructor. */
		array() = default;

		array(const_reference value = T());
		array(std::initializer_list<value_type> ilist);

		/** Default copy constructor */
		array(const array& other) = default;

		array(const std::array<value_type, N>& other);

		/** Default move constructor */
		array(array&& other) = default;

		virtual ~array();
	
	public:
		/********** Public Member Function Declarations **********/

		constexpr size_type len();

		const_reference at(size_type idx) const;
		reference at(size_type idx);

		const_reference front() const;
		reference front();

		const_reference back() const;
		reference back();

		iterator begin();
		iterator end();

		const_iterator cbegin();
		const_iterator cend();

	public:
		/********** Operator Overload Declarations **********/

		const_reference operator[](size_type idx) const;
		reference operator[](size_type idx);

		array<T, N>& operator=(const array<T, N>& other);
		array<T, N>& operator=(array<T, N>&& other);

	public:
		/********** Friend Function **********/

		/**
		 * Stream insertion operator overload for std::string specialization.
		*/
		template<size_t fN>
		friend std::ostream& operator<<(std::ostream& os, const nostl::array<std::string, fN>& rhs) {

			// begin array
			os << "[";

			// print each string
			for (size_t i = 0; i < rhs.m_size; i++) {

				// print string
				os << '"' << rhs.m_data[i] << '"';

				// if there sre still strings to print, print a comma
				if (i + 1 < rhs.m_size) {
					os << ", ";
				}
			}

			// end array
			os << "]";

			// return reference to output stream
			return os;
		}
	
		/**
		 * Default stream insertion operator overload for any specialization.
		*/
		template<typename fT, size_t fN>
		friend std::ostream& operator<<(std::ostream& os, const nostl::array<fT, fN>& rhs) {

			// begin array
			os << "[";

			// print each element
			for (size_t i = 0; i < rhs.m_size; i++) {

				// check if T is of a primitive type
				if (std::is_arithmetic<T>::value) {
					// print element
					os << rhs.m_data[i];
				} else {
					// print element surrounded by brackets
					os << "{ " << rhs.m_data[i] << " }";
				}

				// if there sre still elements to print, print a comma
				if (i + 1 < rhs.m_size) {
					os << ", ";
				}
			}

			// end array
			os << "]";

			// return reference to output stream
			return os;
		}
		
	}; // class array

} // namespace nostl

#endif // NOSTL_ARRAY
