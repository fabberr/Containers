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
#define ARG_PRGRM 		(argv[0]) 	/** Program name. */
#define ARG_CONTAINER 	(argv[1]) 	/** <container> command line argument. */
#define ARG_TEST 		(argv[2]) 	/** <test> command line argument. */

// maps <test> cmd line args to their respective nostl::vector unit tests, only visible in this translation unit
static parser::cllbkmap_t vector_callbacks {
	{ "constructors"	, test::vector::constructors_and_assignment_operations     }, 
	{ "std-constructors", test::vector::constructors_and_assignment_operations_std }, 
	{ "compare"			, test::vector::compare                                    }
};

// maps <test> cmd line args to their respective nostl::array unit tests, only visible in this translation unit
static parser::cllbkmap_t array_callbacks {
	{ "not-implemented", nullptr }
};

// maps <container> cmd line args to their respective unit test callback maps, only visible in this translation unit
static parser::containers_t containers {
	{ "vector", vector_callbacks }, 
	{ "array" , array_callbacks  }
};

// unit test to run
parser::unit_t run_test; // declared here, linked externally in main.cpp

/********** Command Line Parser Functions (definitions) **********/

/**
 * Outputs help message to an std::ostream (std::cerr by default).
*/
void parser::help(const char* argv[], std::ostream& output) {
	const std::string& program = fs::path(ARG_PRGRM).stem().string();
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
 * Parses command line arguments and sets control variables accordingly.
*/
void parser::parse(int argc, const char* argv[]) {

	// parsing optional args, if any are present, program exits after operation
	for (std::size_t arg_idx = 1; arg_idx < argc; ++arg_idx) {
		// for each arg, search for optional args
		parser::arg_t arg = argv[arg_idx];
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
	if (cllbkmap_t& cllbck_map = containers[ARG_CONTAINER]; !(cllbck_map.empty())) {
		// parsing <test> argument
		if (const unit_t& cllbk = cllbck_map[ARG_TEST]; cllbk) {
			// set control variable
			run_test = cllbk;
		} else {
			// error
			std::fprintf(stderr, "[ERROR] container \"%s\" has no test \"%s\" defined\n", ARG_CONTAINER, ARG_TEST);
			std::exit(-1);
		}
	} else {
		// error
		std::fprintf(stderr, "[ERROR] container \"%s\" does not exist\n", ARG_CONTAINER);
		std::exit(-1);
	}
}

/** @todo --brief option */
