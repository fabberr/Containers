/********** Included Headers **********/

// C++ library
#include <iostream> 		// std::cout, std::cerr, std::fprintf
#include <filesystem> 		// std::filesystem namespace
#include <functional> 		// std::function

// filesystem namespace alias
namespace fs = std::filesystem;

// C library
#include <cstring> 	// std::strncmp
#include <cstdlib> 	// std::exit

// internal
#include "../include/unit_tests.h" // declarations of test function

/********** Program Control Logic **********/

// macros
#define CONTAINER_IDX 	1 	// index of <container> argument in arg list
#define TEST_IDX 		2 	// index of <test> argument in arg list

// control variables
enum class containers { VECTOR, ARRAY } container; 	// indicates which cantainer to use
std::function<int(void)> run_test = nullptr; 		// function pointer to specified unit test

/**
 * Parses command line arguments and sets program control variables accordingly.
*/
void parse(int argc, const char* const * argv) {
	
	// checking arg count
	if (argc < 3) {
		const auto program = fs::path(argv[0]).filename().string();
		std::cerr << 
			"Usage: " << program << " <container> <test> [OPTIONS]\n"
			"\nOPTIONS:\n"
			"  --usage, --help, -h, -?:\n"
			"    Displays this help message.\n"
			"  --list:\n"
			"    Dumps a list of available containers and their respective tests." 
		<< std::endl;
		std::exit(-1);
	}

	// parsing <container> argument
	if (std::strncmp(argv[CONTAINER_IDX], "vector", 6) == 0) {
		// set container to nostl::vector
		container = containers::VECTOR;

		// parsing <test> argument
		if (std::strncmp(argv[TEST_IDX], "constructors", 12) == 0) {
			run_test = test::vector::constructors_and_assignment_operations;
		} else if (std::strncmp(argv[TEST_IDX], "std_constructors", 16) == 0) {
			run_test = test::vector::constructors_and_assignment_operations_std;
		} else if (std::strncmp(argv[TEST_IDX], "compare", 7) == 0) {
			run_test = test::vector::compare;
		} else {
			// error
			std::fprintf(stderr, "[ERROR]: container \"%s\" has no test \"%s\" defined", argv[CONTAINER_IDX], argv[TEST_IDX]);
		}
	} else if (std::strncmp(argv[CONTAINER_IDX], "array", 5) == 0){
		// set container to nostl::array
		container = containers::ARRAY;

		// map nostl::array tests
		/** @todo add tests for nostl::array */
	} else {
		// error
		std::fprintf(stderr, "[ERROR]: container \"%s\" does not exist\n", argv[CONTAINER_IDX]);
		std::exit(-1);
	}
}

/********** Main Entry Point **********/

int main(int argc, char** argv) {

	// parse cmd line args, resolves which test to run
	parse(argc, argv);

	// run unit test (if any was resolved) and return its value as exit code
	if(run_test) return run_test();

	// if this point was reached, the parser could not resolve which test to run, return error code
	return -1;
}

/** @todo --help */
/** @todo --list */
/** @todo move cmd line parser and program control logic to a separate file (make global vars static to access them across translation units) */
