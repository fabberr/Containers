/********** Included Headers **********/

// C++ library
#include <iostream>	// std::cout
#include <utility> 	// std::move
#include <string> 	// std::string
#include <vector> 	// std::vector
#include <array> 	// std::array

// libnostl
#include <nostl/vector.h> 	// nostl::vector
#include <nostl/array.h> 	// nostl::array

/********** Functions **********/

/**
 * Prints a nostl::vector along with information about it's memory footprint to 
 * the standard character output.
*/
template<typename T, size_t N>
void print_vec_stats(const nostl::vector<T, N>& vec) {
	
	// print vector
	std::cout << vec << std::endl;
	
	// print memory usage information
	std::printf(
		"  len=%d elements, capacity=%d elements, elem_size=%d bytes\n", 
		vec.len(), 
		vec.capacity(), 
		sizeof (T)
	);
	std::printf(
		"  mem_usage=%d bytes, total_allocated_mem=%d bytes, unused_mem=%d bytes\n", 
		vec.memsize(), 
		vec.allocsize(), 
		vec.allocsize() - vec.memsize()
	);
}

/********** Main Entry Point **********/

template<typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const std::array<T, N>& rhs) {
	os << '[';
	typename std::array<T, N>::const_iterator it = rhs.begin();
	while (it != rhs.end()) {
		os << *it;
		if (++it != rhs.end()) {
			os << ", ";
		}
	}
	os << ']';
	return os;
}

int main() {

	using arr_t = nostl::array<int, 10>;
	arr_t arr { 10, 20, 30, 40, 50 };

	const nostl::array deduced { 1, 2, 3, 4, 5 };

	std::cout << "arr: " << arr << std::endl;
	std::cout << "deduced: " << deduced << std::endl;
	
	return 0;
}
