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

	// testing iterators and initializer lists
	nostl::vector<std::string> vecStrings{ "vector", "of", "strings", "with", "initializer", "list" };
	vecStrings.emplace_back("iwtcits");
	std::cout << "vecStrings (operator<<):\n" << vecStrings << std::endl;

	// range-based for loop
	std::cout << "\nvecStrings (rage-based for loop):" << std::endl;
	for (const auto& str : vecStrings) {
		std::cout << str << std::endl;
	}

	// iterator
	std::cout << "\nvecStrings (iterator):" << std::endl;
	using itr_t = nostl::vector<std::string>::iterator;
	for (itr_t it = vecStrings.begin(); it != vecStrings.end(); it++) {
		std::cout << *it << std::endl;
	}

	// modify contents using iterators
	std::cout << "\nmodifying every string except the last one" << std::endl;
	for (itr_t it = vecStrings.begin(); it != vecStrings.end() - 1; it++) {
		*it += " aaaa";
	}
	std::cout << "vecStrings (modified): " << vecStrings << std::endl;

	std::cout << "\naddresses:" << std::endl;
	for (itr_t it = vecStrings.begin(); it != vecStrings.end(); it++) {
		std::cout << it.get_ptr() << ": \"" << *it << '\"' << std::endl;
	}

	// // reverse iterator
	// std::cout << "\nvecStrings (reverse iterator):" << std::endl;
	// using ritr_t = nostl::vector<std::string>::reverse_iterator;
	// for (ritr_t it = vecStrings.begin(); it != vecStrings.end(); it++) {
	// 	std::cout << *it << std::endl;
	// }

	return 0;
}

/** @todo test const iterators */
/** @todo test vector::resize */
