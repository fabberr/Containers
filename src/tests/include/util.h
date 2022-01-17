#ifndef LIBNOSTL_TESTS_UTIL_H
#define LIBNOSTL_TESTS_UTIL_H

/********** Included Headers **********/

// C++ library
#include <iostream> // std::ostream
#include <string> 	// std::string

// libnostl
#include <nostl/vector.h> // nostl::vector

/********** util.h **********/

namespace util {

	/********** Utility Functions (definitions) **********/

	template<typename T, size_t N>
	void print_vec_stats(const nostl::vector<T, N>& vec);

} // namespace util

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& rhs);

#endif // LIBNOSTL_TESTS_UTIL_H
