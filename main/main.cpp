/********** Headers **********/

// C++ library
#include <iostream>	// std::cout
#include <utility> 	// std::move
#include <string> 	// std::string
#include <vector> 	// std::vector

// libnostl
#include <nostl/vector.h> // nostl::vector

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

int main() {

	using svec_t = nostl::vector<std::string>;
	svec_t v1{ "testing", "initializer", "list", "constructor", "of", "nostl::vector" };
	svec_t v2(10, "initialized");

	std::cout << "v1: " << v1 << std::endl;
	std::cout << "\nv2: " << v2 << " len=" << v2.len() << std::endl;

	return 0;
}
