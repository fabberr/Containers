/********** Included Headers **********/

// C++ library
#include <iostream> 		// std::cout, std::cerr, std::fprintf
#include <string> 			// std::string
#include <map> 				// std::map
#include <functional> 		// std::function
#include <filesystem> 		// filesystem utilities

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

// A c-style string comparison functor
struct cstring_cmp {
	bool operator()(const char* lhs, const char* rhs) const {
	   return ((lhs && rhs) && (std::strcmp(lhs, rhs) < 0)); // strcmp could cause a memory leak, look into a way of using strncmp here
   }
};

// type aliases
using unit_t = std::function<int(void)>; 					// unit test signature: int return type, no parameters
using map_t = std::map<const char*, unit_t, cstring_cmp>; 	// type of [key:value] pair, comparison functor

// mapping available nostl::vector tests to their respective command line args
map_t vector_callbacks {
	{ "constructors"	, test::vector::constructors_and_assignment_operations 		}, 
	{ "std-constructors", test::vector::constructors_and_assignment_operations_std 	}, 
	{ "compare"			, test::vector::compare 									}
};

// mapping available nostl::array tests to their respective command line args
map_t array_callbacks { { "not-implemented", nullptr } };

/**
 * Prints help message to standard character output.
*/
void help(const char* argv[]) {
	const auto program = fs::path(argv[0]).filename().string();
	std::cerr << 
		"Usage: " << program << " <container> <test> [OPTIONS]\n"
		"\nOPTIONS:\n"
		"  --usage, --help, -h, -?:\n"
		"    Displays this help message.\n"
		"  --list:\n"
		"    Dumps a list of available containers and their respective tests." 
	<< std::endl;
}

/**
 * Parses the second command line argument <test>.
*/
void parse_test(map_t& callback_map, const char* argv[]) {

	// Check if key exists in callback map ad retrieve it.
	// If no key by the name of specified <test> cmd line arg is found, a new key-value 
	// pair is inserted with the `second` member as a default-initialized unit_t (an 
	// "empty" std::function that points to nullptr), which is then returned as a 
	// reference.
	if (const unit_t& cllbk = callback_map[argv[TEST_IDX]]; cllbk) { // assign to `cllbk` then check if not null
		// run unit test
		cllbk();
	} else {
		// error
		std::fprintf(stderr, "[ERROR]: container \"%s\" has no test \"%s\" defined", argv[CONTAINER_IDX], argv[TEST_IDX]);
	}
}

/**
 * Parses command line arguments and sets program control variables accordingly.
*/
void parse(int argc, const char* argv[]) {

	// checking arg count
	if (argc < 3) {
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
		std::fprintf(stderr, "[ERROR]: container \"%s\" does not exist\n", argv[CONTAINER_IDX]);
		std::exit(-1);
	}
}

/********** Main Entry Point **********/

int main(int argc, char* argv[]) {

	// parse cmd line args, resolves which test to run
	parse(argc, (const char**)argv);

	// run unit test (if any was resolved) and return its value as exit code
	// if(run_test) return run_test();

	// if this point was reached, the parser could not resolve which test to run, return error code
	return -1;
}

/** @todo --help */
/** @todo --list */
/** @todo move cmd line parser and program control logic to a separate file (make global vars static to access them across translation units) */
