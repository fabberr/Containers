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
#include <cstdint> // std::int32_t

// internal
#include "../include/cmdline_parser.h" 	// command line parser types and functions declarations
#include "../include/unit_tests.h" 		// test function declarations

/********** Program Control Logic **********/

// macros
#define ARG_EXECUTABLE 	(argv[0]) 	/** Executable name. */
#define ARG_CONTAINER 	(argv[1]) 	/** <container> command line argument. */
#define ARG_TEST 		(argv[2]) 	/** <test> command line argument. */

// function-like macros
#define MSG_AND_EXIT_FAILURE(msg, ...) std::fprintf(stderr, msg __VA_OPT__(,) __VA_ARGS__); std::exit(static_cast<std::int32_t>(-1));

/**
 * Maps <test> cmd line args to their respective nostl::vector unit tests.
 * Only visible in this translation unit.
*/
static parser::cllbkmap_t vector_callbacks {
	{ "initialization"	  , test::vector::initialization     }, 
	{ "initialization-std", test::vector::initialization_std }, 
	{ "compare"			  , test::vector::compare            }
};

/**
 * Maps <test> cmd line args to their respective nostl::array unit tests.
 * Only visible in this translation unit.
*/
static parser::cllbkmap_t array_callbacks {
	{ "not-implemented", nullptr }
};

/**
 * Maps <container> cmd line args to their respective unit test 
 * callback maps.
 * Only visible in this translation unit.
*/
static parser::containers_t containers {
	{ "vector", vector_callbacks }, 
	{ "array" , array_callbacks  }
};

/**
 * Unit test to run.
 * The std::function object is declared in this translation unit but linked 
 * externally in main.cpp, where the test will be invoked.
*/
parser::unit_t run_test = nullptr;

/********** Command Line Parser Functions (definitions) **********/

/**
 * Outputs help message to an std::ostream (std::cerr by default).
*/
void parser::help(const char* argv[], std::ostream& out) {
	const std::string& program = fs::path(ARG_EXECUTABLE).stem().string();
	out << 
		"Usage: ./" << program << " <container> <test> [OPTIONS]\n"
		"\n"
		"OPTIONS:\n"
		"  --usage, --help, -h, -?:\n"
		"    Displays this help message.\n"
		"\n"
		"  --list:\n"
		"    Dumps a list of available containers and their respective tests to standard\n"
		"    character output.\n"
		"\n"
		"  --brief:\n"
		"    Same as --list but also displays a brief description."
	<< std::endl;
}

/**
 * Dumps a list of available containers and their respective tests to standard 
 * character output.
*/
void parser::list() {

	// for each container
	for (const auto& [container, cllbk_map] : containers) {
		// print name of container followed by the name of its available tests
		std::cout << container << '\n';
		for (const auto& [test, function] : cllbk_map) {
			std::cout << "  " << test << '\n';
		}
	}
	std::cout.flush();
}

/**
 * Dumps a list of available containers and their respective tests to standard 
 * character output with brief descriptions for each unit test.
*/
void parser::brief() {

	std::cout << 
		"vector\n"
		"|\n"
		"+---initialization\n"
		"|       Tests constructors and assignment operations.\n"
		"|       See `./tests/src/unit_tests/unit_tests.cpp:31` for more info.\n"
		"|\n"
		"+---initialization-std\n"
		"|       Tests constructor and copy assignment operation from std::vector.\n"
		"|       See `./tests/src/unit_tests/unit_tests.cpp:86` for more info.\n"
		"|\n"
		"\\---compare\n"
		"        Tests equality and inequality operators.\n"
		"        See `./tests/src/unit_tests/unit_tests.cpp:124` for more info."
	<< std::endl;
}

/**
 * Parses command line arguments and sets control variables accordingly.
*/
void parser::parse(int argc, const char* argv[]) {

	// parsing optional args, if any are present, program exits after operation
	for (std::size_t arg_idx = 1; arg_idx < argc; ++arg_idx) {
		// for each arg, check if any of the optional args are present
		parser::arg_t arg = argv[arg_idx];
		if ((std::strncmp(arg    , "--usage", 7) == 0) 	|| 	// --usage
			(std::strncmp(arg + 1, "-h"     , 2) == 0) 	|| 	// --help, -h, among other such substrings lmao
			(std::strncmp(arg    , "-?"     , 2) == 0) 		// -?
		) {
			// output help to cout, terminate program successfully
			parser::help(argv, std::cout);
			std::exit(0);
		} else if (std::strncmp(arg, "--list", 6) == 0) { 	// --list
			// dump list of containers and tests to cout, terminate program successfully
			parser::list();
			std::exit(0);
		} else if (std::strncmp(arg, "--brief", 7) == 0) { 	// --brief
			// dump list of containers and tests with brief description to cout, terminate program successfully
			parser::brief();
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
			MSG_AND_EXIT_FAILURE("[ERROR] container \"%s\" has no test \"%s\" defined\n", ARG_CONTAINER, ARG_TEST)
		}
	} else {
		MSG_AND_EXIT_FAILURE("[ERROR] container \"%s\" does not exist\n", ARG_CONTAINER)
	}
}
