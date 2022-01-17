/********** Included Headers **********/

// C++ library
#include <iostream> 	// std::ostream, std::cout, std::fprintf, std::endl
#include <string> 		// std::string
#include <filesystem> 	// filesystem utilities

// filesystem namespace alias
namespace fs = std::filesystem;

// C library
#include <cstddef> // std::size_t
#include <cstring> // std::strncmp
#include <cstdlib> // std::exit

// internal
#include "../include/cmdline_parser.h" 	// command line parser types and functions declarations
#include "../include/unit_tests.h" 		// test function declarations

/********** Program Control Logic **********/

// macros
#define PGRM_IDX 		0 	// index of program name in arg list
#define CONTAINER_IDX 	1 	// index of <container> argument in arg list
#define TEST_IDX 		2 	// index of <test> argument in arg list

// maps available nostl::vector unit tests to their respective command line args
parser::map_t vector_callbacks {
	{ "constructors"	, test::vector::constructors_and_assignment_operations 		}, 
	{ "std-constructors", test::vector::constructors_and_assignment_operations_std 	}, 
	{ "compare"			, test::vector::compare 									}
};

// maps available nostl::array tests to their respective command line args
parser::map_t array_callbacks{};

// unit test to run
parser::unit_t run_test; // accessible across translation units, used in main.cpp

/********** Command Line Parser Functions (definitions) **********/

/**
 * Outputs help message to an std::ostream (std::cerr by default).
*/
void parser::help(const char* argv[], std::ostream& output) {
	const std::string& program = fs::path(argv[PGRM_IDX]).stem().string();
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
void parser::list() {
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
 * Parses the second command line argument <test> to determine which unit test 
 * should be run.
*/
void parser::parse_test(map_t& callback_map, const char* argv[]) {

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
void parser::parse(int argc, const char* argv[]) {

	// parsing optional args, if any are present, program exits after operation
	for (std::size_t arg_idx = 1; arg_idx < argc; ++arg_idx) {
		// for each arg, search for optional args
		const char* arg = argv[arg_idx];
		if ((std::strncmp(arg    , "--usage", 7) == 0) 	|| 	// --usage
			(std::strncmp(arg + 1, "-h"     , 2) == 0) 	|| 	// --help, -h, among other such substrings lmao
			(std::strncmp(arg    , "-?"     , 2) == 0) 		// -?
		) {
			// output help to cout, terminate program successfully
			parser::help(argv, std::cout);
			std::exit(0);
		} else if (std::strncmp(arg, "--list", 6) == 0) { // --list
			// dump available containers and tests to cout, terminate program successfully
			parser::list();
			std::exit(0);
		}
	}

	// checking arg count
	if (argc < 3) {
		// invalid number of arguments: output help to cerr, terminate program with code -1
		parser::help(argv);
		std::exit(-1);
	}

	// parsing <container> argument
	if (std::strncmp(argv[CONTAINER_IDX], "vector", 6) == 0) {
		// parsing <test> argument with callback map for nostl::vector tests
		parser::parse_test(vector_callbacks, argv);
	} else if (std::strncmp(argv[CONTAINER_IDX], "array", 5) == 0) {
		// parsing <test> argument with callback map for nostl::array tests
		parser::parse_test(array_callbacks, argv);
	} else {
		// error
		std::fprintf(stderr, "[ERROR] container \"%s\" does not exist\n", argv[CONTAINER_IDX]);
		std::exit(-1);
	}
}

/** @todo generate --list programatically array of entries (entry -> container arg + map of unit tests) */
