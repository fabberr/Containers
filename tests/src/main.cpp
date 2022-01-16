/********** Included Headers **********/

// C++ library
#include <iostream> // std::cout

// internal
#include "../include/unit_tests.h" // test function declarations

/********** Main Entry Point **********/

int main(int argc, char* argv[]) {
	test::vector::constructors_and_assignment_operations();
	// test::vector::constructors_and_assignment_operations_std();
	// test::vector::compare();
	return 0;
}

/** @todo write application control logic for executing correct text from cmd line */
/** @todo convert header files into C++20 modules maybe */
