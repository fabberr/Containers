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

	/********** Type Definitions **********/
	
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

	// type aliases
	using unit_t = std::function<int(void)>; 					// unit test signature: int return type, no parameters
	using map_t = std::map<const char*, unit_t, cstring_cmp>; 	// maps a test's cmd line arg to it's respective function pointer, provides a user-defined comparator
	
	/********** Command Line Parser Functions (declarations) **********/

	void help(const char* argv[], std::ostream& output = std::cerr);
	void list();
	void parse_test(map_t& callback_map, const char* argv[]);
	void parse(int argc, const char* argv[]);

} // namespace parser

#endif // LIBNOSTL_TESTS_CMDLINE_PARSER_H
