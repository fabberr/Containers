/********** Included Headers **********/

// internal
#include "../include/cmdline_parser.h" // parser::unit_t, parser_parse

/********** Program Control Logic **********/

// unit test to run
parser::unit_t run_test; // accessible across translation units, used in cmdline_parser.cpp

/********** Main Entry Point **********/

int main(int argc, char* argv[]) {

	// parse cmd line args and sets control variable `run_test`, if no test was resolved from the args, the program exits with code -1
	parser::parse(argc, (const char**)argv);

	// return status code from test's execution
	return run_test();
}
