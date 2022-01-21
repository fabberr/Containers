#ifndef LIBNOSTL_TESTS_CMDLINE_PARSER_H
#define LIBNOSTL_TESTS_CMDLINE_PARSER_H

/********** Included Headers **********/

// C++ library
#include <iostream> 	// std::ostream, std::cerr
#include <functional> 	// std::function
#include <map> 			// std::map

// C library
#include <cstring> // std::strcmp

/********** cmdline_parser.h **********/

namespace parser {

	/********** Functors **********/
	
	/**
	 * A c-style string comparison functor, assumes the strings are null-terminated.
	 * Allows member access through std::map::operator[] using a null-terminated 
	 * character string as key.
	*/
	struct cstring_cmp {
		/** Comparison predicate, compares a string to a key in the map. */
		bool operator()(const char* lhs, const char* rhs) const {
			return (
				(lhs && rhs) && 			// nullptr checks
				(std::strcmp(lhs, rhs) < 0) // comparing strings
			); // FIXME: strcmp could cause a buffer overflow, look into a way of using strncmp here instead
		}
	};

	/********** Type Definitions **********/

	typedef const char* 								arg_t; 			/** Command line argument type. A C-style null-terminated character string. */
	typedef std::function<int(void)> 					unit_t; 		/** Unit test function pointer type. */
	typedef std::map<arg_t, unit_t, cstring_cmp> 		cllbkmap_t; 	/** Callback map type. Maps <test> cmd line args to their respective unit tests. */
	typedef std::map<arg_t, cllbkmap_t, cstring_cmp> 	containers_t; 	/** Container map type. Maps <container> cmd line args to their respective callback maps. */

	/********** Command Line Parser Functions (declarations) **********/

	void help(const char* argv[], std::ostream& out = std::cerr);
	void list();
	void brief();
	void parse(int argc, const char* argv[]);

} // namespace parser

#endif // LIBNOSTL_TESTS_CMDLINE_PARSER_H
