/********** Included Headers **********/

// C++ library
#include <iostream>		// std::cout, std::printf
#include <utility> 		// std::move
#include <string> 		// std::string
#include <vector> 		// std::vector
#include <array> 		// std::array

// libnostl
#include <nostl/vector.h> 	// nostl::vector
#include <nostl/array.h> 	// nostl::array

// internal
#include "../include/util.h" 		// declarations for utility functions
#include "../include/unit_tests.h" 	// declarations of test functions

/********** Unit Tests (definition) **********/

/**
 * arg: constructors
 * Unit Test: constructors and assignment operations.
 *   - initializer list constructor
 *   - copy constructor
 *   - move constructor
 *   - copy assignment
 *   - move assignment
 * 
 * returns 0 if test succeeds, otherwise, a negative value.
*/
int test::vector::constructors_and_assignment_operations() {

	// array type alias
	using arr_t = nostl::vector<std::string, 10>;

	// access to std::string literals
	using namespace std::string_literals;

	// testing initializer list constructor
	std::cout << "constructing base object...\n";
	const arr_t base{ "0"s, "1"s, "2"s, "3"s, "4"s, "5"s, "6"s, "7"s, "8"s, "9"s };
	std::cout << "base: " << base << '\n'; 								// base: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

	// testing copy constructor
	std::cout << "--------------------\n";
	std::cout << "copying base into copy_constructed...\n";
	arr_t copy_constructed(base);
	std::cout << "base: " << base << '\n'; 								// base: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	std::cout << "copy_constructed: " << copy_constructed << std::endl; // copy_constructed: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

	// testing move constructor
	std::cout << "--------------------\n";
	std::cout << "moving copy_constructed into move_contructed...\n";
	arr_t move_constructed(std::move(copy_constructed));
	std::cout << "copy_constructed: " << copy_constructed << '\n'; 		// copy_constructed: []
	std::cout << "move_constructed: " << move_constructed << std::endl; // move_constructed: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

	// testing copy assignment operation
	std::cout << "--------------------\n";
	std::cout << "copying base into copy_assigned...\n";
	arr_t copy_assigned;
	copy_assigned = base;
	std::cout << "base: " << base << '\n'; 							// base: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	std::cout << "copy_assigned: " << copy_assigned << std::endl; 	// copy_assigned: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

	// testing move assignment operation
	std::cout << "--------------------\n";
	std::cout << "moving copy_assigned into move_assigned...\n";
	arr_t move_assigned;
	move_assigned = std::move(copy_assigned);
	std::cout << "copy_assigned: " << copy_assigned << '\n'; 		// copy_assigned: []
	std::cout << "move_assigned: " << move_assigned << std::endl; 	// move_assigned: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

	// status code to be returned in main
	return 0;
}

/**
 * arg: std-constructors
 * Unit Test: constructors and assignment (from std::vector) operations.
 *   - copy (from std::vector) constructor
 *   - copy (from std::vector) assignment
 * 
 * returns 0 if test succeeds, otherwise, a negative value.
*/
int test::vector::constructors_and_assignment_operations_std() {

	// array type alias
	using arr_t = nostl::vector<std::string, 10>;

	// access to std::string literals
	using namespace std::string_literals;

	// testing initializer list constructor
	std::cout << "constructing base object...\n";
	const std::vector<std::string> base{ "0"s, "1"s, "2"s, "3"s, "4"s, "5"s, "6"s, "7"s, "8"s, "9"s };
	std::cout << "base: " << base << std::endl;							// base: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

	// testing copy constructor
	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << "copying base into copy_constructed...\n";
	arr_t copy_constructed(base);
	std::cout << "base: " << base << '\n'; 								// base: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	std::cout << "copy_constructed: " << copy_constructed << std::endl; // copy_constructed: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

	// testing copy assignment operation
	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << "copying base into copy_assigned...\n";
	arr_t copy_assigned;
	copy_assigned = base;
	std::cout << "base: " << base << '\n'; 								// base: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	std::cout << "copy_assigned: " << copy_assigned << std::endl; 		// copy_assigned: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

	// status code to be returned in main
	return 0;
}

/**
 * arg: compare
 * Unit Test: comparison operator overloads.
 *   - operator==
 *   - operator!=
*/
int test::vector::compare() {

	// type aliases
	using vec_t = nostl::vector<int>;
	using itr_t = vec_t::iterator;

	// insert boolean values into standard character output
	const auto& fmtflags = std::cout.flags(); 	// save format control flags
	std::cout << std::boolalpha; 				// sets boolapha flag for std::cout

	// testing operator==
	vec_t v1{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	vec_t v2(v1);
	std::cout << "v1: " << v1 << '\n'; // v1: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	std::cout << "v2: " << v2 << '\n'; // v2: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	std::cout << "comparing v1 and v2: match=" << (v1 == v2) << std::endl;

	// testing operator!=
	vec_t v3; // inverse of v1
	for (itr_t it = v1.end(); it != v1.begin(); --it) {
		v3 += *(it - 1);
	}
	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << "v1: " << v1 << '\n'; // v1: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	std::cout << "v3: " << v3 << '\n'; // v3: [9, 8, 7, 6, 5, 4, 3, 2, 1, 0]
	std::cout << "comparing v1 and v3: match=" << !(v3 != v1) << std::endl;

	std::cout.flags(fmtflags); // restore format control flags
	
	// status code to be returned in main
	return 0;
}

/** @todo add checks to validate the tests (currently only validating if program exits normally) */
