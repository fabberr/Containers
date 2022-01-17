/********** Included Headers **********/

// C++ library
#include <iostream> 	// std::cout, std::endl, std::cerr, std::fprintf
#include <map> 			// std::map
#include <functional> 	// std::function
#include <filesystem> 	// filesystem utilities

// filesystem namespace alias
namespace fs = std::filesystem;

// C library
#include <cstddef> 	// std::size_t
#include <cstring> 	// std::strcmp, std::strncmp
#include <cstdlib> 	// std::exit

// internal
#include "../include/unit_tests.h" // test function declarations

/********** Program Control Logic **********/

// macros
#define CONTAINER_IDX 	1 	// index of <container> argument in arg list
#define TEST_IDX 		2 	// index of <test> argument in arg list

/**
 * A c-style string comparison functor, assumes the strings are null-terminated.
 * Allows member access through std::map::operator[] using a null-terminated 
 * character string as key.
*/
struct cstring_cmp {
	/** Comparison predicate, compares a string to a key in the map. */
	bool operator()(const char* lhs, const char* rhs) const {
	   return (
		   (lhs && rhs) && 				// nullptr checks
		   (std::strcmp(lhs, rhs) < 0) 	// comparing strings
		); // FIXME: strcmp could cause a buffer overflow, look into a way of using strncmp here instead
   }
};

// type aliases
using unit_t = std::function<int(void)>; 					// unit test signature: int return type, no parameters
using map_t = std::map<const char*, unit_t, cstring_cmp>; 	// maps a test's cmd line arg to it's respective function pointer, provides a user-defined comparator

// maps available nostl::vector tests to their respective command line args
map_t vector_callbacks {
	{ "constructors"	, test::vector::constructors_and_assignment_operations 		}, 
	{ "std-constructors", test::vector::constructors_and_assignment_operations_std 	}, 
	{ "compare"			, test::vector::compare 									}
};

// maps available nostl::array tests to their respective command line args
map_t array_callbacks{};

// unit test to run
unit_t run_test = nullptr;

/**
 * Outputs help message to an std::ostream (std::cerr by default).
*/
void help(const char* argv[], std::ostream& output = std::cerr) {
	const auto program = fs::path(argv[0]).stem().string();
	output << 
		"Usage: ./" << program << " <container> <test> [OPTIONS]\n"
		"\n"
		"OPTIONS:\n"
		"  --usage, --help, -h, -?:\n"
		"    Displays this help message.\n"
		"\n"
		"  --list:\n"
		"    Dumps a list of available containers and their respective tests to standard\n"
		"    character output." 
	<< std::endl;
}

/**
 * Dumps a list of available containers and their respective tests to standard 
 * character output.
*/
void list() {
	std::cout << 
		"List of available containers and unit tests tests for libnostl:\n"
		"\n"
		"vector\n"
		"|\n"
		"+---constructors\n"
		"|       Tests constructors and assignment operations.\n"
		"|       See `./tests/src/unit_tests/unit_tests.cpp:31` for more info.\n"
		"|\n"
		"+---std-constructors\n"
		"|       Tests constructor and copy assignment operation from std::vector.\n"
		"|       See `./tests/src/unit_tests/unit_tests.cpp:86` for more info.\n"
		"|\n"
		"\\---compare\n"
		"        Tests equality and inequality operators.\n"
		"        See `./tests/src/unit_tests/unit_tests.cpp:124` for more info.\n"
	<< std::endl;
}

/**
 * Parses the second command line argument <test> to determine which test should be run
*/
void parse_test(map_t& callback_map, const char* argv[]) {

	// Checks if a test exists in `callback_map` and retrieves it.
	// If no key specified by <test> cmd line arg is found, a new key:value pair is 
	// inserted with the `second` member as a default-initialized unit_t (an "empty"
	// std::function that points to nullptr), which is then returned as a reference.
	if (const unit_t& cllbk = callback_map[argv[TEST_IDX]]; cllbk) { // assign to `cllbk` then check if not null
		// set control variable
		run_test = cllbk;
	} else {
		// error
		std::fprintf(stderr, "[ERROR] container \"%s\" has no test \"%s\" defined\n", argv[CONTAINER_IDX], argv[TEST_IDX]);
		std::exit(-1);
	}
}

/**
 * Parses command line arguments and sets control variables accordingly.
*/
void parse(int argc, const char* argv[]) {

	// parsing optional args, if any are present, program exits after operation
	for (size_t arg_idx = 1; arg_idx < argc; ++arg_idx) {
		// for each arg, search for optional args
		const char* arg = argv[arg_idx];
		if ((std::strncmp(arg    , "--usage", 7) == 0) 	|| 	// --usage
			(std::strncmp(arg + 1, "-h"     , 2) == 0) 	|| 	// --help, -h, among other such substrings lmao
			(std::strncmp(arg    , "-?"     , 2) == 0) 		// -?
		) {
			// output help to cout, terminate program successfully
			help(argv, std::cout);
			exit(0);
		} else if (std::strncmp(arg, "--list", 6) == 0) { // --list
			// dump available containers and tests to cout, terminate program successfully
			list();
			exit(0);
		}
	}

	// checking arg count
	if (argc < 3) {
		// invalid number of arguments: output help to cerr, terminate program with code -1
		help(argv);
		std::exit(-1);
	}

	// parsing <container> argument
	if (std::strncmp(argv[CONTAINER_IDX], "vector", 6) == 0) {
		// parsing <test> argument with callback map for nostl::vector tests
		parse_test(vector_callbacks, argv);
	} else if (std::strncmp(argv[CONTAINER_IDX], "array", 5) == 0) {
		// parsing <test> argument with callback map for nostl::array tests
		parse_test(array_callbacks, argv);
	} else {
		// error
		std::fprintf(stderr, "[ERROR] container \"%s\" does not exist\n", argv[CONTAINER_IDX]);
		std::exit(-1);
	}
}

/********** Main Entry Point **********/

int main(int argc, char* argv[]) {

	// parse cmd line args and sets control variable `run_test`, if no test was resolved from the args, the program exits with code -1
	parse(argc, (const char**)argv);

	// return status code from test's execution
	return run_test();
}

/** @todo move cmd line parser and program control logic to a separate file (declare global vars as extren to access them from another translation unit) */
