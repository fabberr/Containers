/********** Included Headers **********/

// C++ library
#include <iostream>		// std::cout
#include <utility> 		// std::move
#include <string> 		// std::string
#include <vector> 		// std::vector
#include <array> 		// std::array
#include <functional> 	// std::function
#include <type_traits> 	// std::is_fundamental, std::is_pointer, std::is_member_pointer

// libnostl
#include <nostl/vector.h> 	// nostl::vector
#include <nostl/array.h> 	// nostl::array

/********** Functions **********/

/**
 * Prints a nostl::vector, along with information about it's memory footprint to 
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

int main() {

	nostl::vector collection{ 10, 20, 30, 40, 50 };
	collection.resize(100);
	print_vec_stats(collection);

	return 0;
}

/** @todo C++20 modules maybe */
