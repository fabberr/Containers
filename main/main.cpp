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
 * Prints the contents of a nostl::vector along with information about it's 
 * memory footprint to the standard character output.
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

/**
 * Unit test: constructors and assignment operations.
 * >initializer list constructor
 * >copy constructor
 * >move constructor
 * >copy assignment
 * >move assignment
*/
void test_constructors_and_assignment_operations() {

	// array type alias
	using arr_t = nostl::vector<std::string, 10>;

	// access to std::string literals
	using namespace std::string_literals;

	// testing initializer list constructor
	std::cout << "--------------------\n";
	std::cout << "constructing base object...\n";
	const arr_t base{ "0"s, "1"s, "2"s, "3"s, "4"s, "5"s, "6"s, "7"s, "8"s, "9"s };
	std::cout << "base: " << base << std::endl; 						// base: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

	// testing copy constructor
	std::cout << "--------------------\n";
	std::cout << "copying base into copy_constructed...\n";
	arr_t copy_constructed{base};
	std::cout << "base: " << base << '\n'; 								// base: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	std::cout << "copy_constructed: " << copy_constructed << std::endl; // copy_constructed: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

	// testing move constructor
	std::cout << "--------------------\n";
	std::cout << "moving copy_constructed into move_contructed...\n";
	arr_t move_constructed{std::move(copy_constructed)};
	std::cout << "copy_constructed: " << copy_constructed << '\n'; 		// copy_constructed: []
	std::cout << "move_constructed: " << move_constructed << std::endl; // move_constructed: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

	// testing copy assignment operation
	std::cout << "--------------------\n";
	std::cout << "copying base into copy_assigned...\n";
	arr_t copy_assigned = base;
	std::cout << "base: " << base << '\n'; 							// base: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	std::cout << "copy_assigned: " << copy_assigned << std::endl; 	// copy_assigned: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

	// testing move assignment operation
	std::cout << "--------------------\n";
	std::cout << "moving copy_assigned into move_assigned...\n";
	arr_t move_assigned = std::move(copy_assigned);
	std::cout << "copy_assigned: " << copy_assigned << '\n'; 		// copy_assigned: []
	std::cout << "move_assigned: " << move_assigned << std::endl; 	// move_assigned: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
}

/********** Main Entry Point **********/

int main() {
	test_constructors_and_assignment_operations();
	return 0;
}

/** @todo REMOVE ITERATORS AND REWIRITE THAT FUCKING MESS FROM SCRATCH */
/** @todo lrn2 unit test using CMake */
/** @todo C++20 modules maybe */
