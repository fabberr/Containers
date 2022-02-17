#ifndef LIBNOSTL_TESTS_CMDLINE_PARSER_H
#define LIBNOSTL_TESTS_CMDLINE_PARSER_H

/********** Included Headers **********/

// C++ library
#include <iostream> 		// std::ostream, std::cerr
#include <functional> 		// std::function, std::hash
#include <unordered_map> 	// std::unordered_map
#include <string_view> 		// std::string_view

// C library
#include <cstring> 	// std::strcmp
#include <cstddef> 	// std::size_t

/********** cmdline_parser.h **********/

namespace parser {

	/********** Functors **********/
	
	/**
	 * A C-style null-terminated character string comparison functor. Checks if two
	 * C-strings match.
	 * Allows for proper use of an std::unordered_map object that uses C-strings for
	 * its keys.
	*/
	struct cstring_cmp {
		bool operator()(const char* lhs, const char* rhs) const {
			return (
				(lhs && rhs) && 			// nullptr checks
				(!std::strcmp(lhs, rhs)) 	// check if strings match
			);
		}
	};
	
	/**
	 * A C-style null-terminated character string hashing function. Encapsulates a 
	 * C-string into an std::string_view object to make use of std::hash possible.
	 * Allows for proper use of an std::unordered_map object that uses C-strings for
	 * its keys.
	*/
	struct cstring_hash {
		std::size_t operator()(const char* str) const {
			return std::hash<std::string_view>{}(std::string_view(str)); // brace-initialize unamed hash object and invoke operator()
		}
	};

	/********** Type Definitions **********/

	/** Command line argument type. */
	typedef const char* arg_t;

	/** Unit test function pointer type. */
	typedef std::function<int(void)> unit_t;

	/** Unit test callback map type. Maps <test> cmd line args to their respective unit test functions. */
	typedef std::unordered_map<arg_t, unit_t, cstring_hash, cstring_cmp> cllbkmap_t;
	
	/** Container map type. Maps <container> cmd line args to their respective unit test callback maps (map of maps). */
	typedef std::unordered_map<arg_t, cllbkmap_t, cstring_hash, cstring_cmp> containers_t;

	/********** Command Line Parser Functions (declarations) **********/

	void help(const char* argv[], std::ostream& out = std::cerr);
	void list();
	void brief();
	void parse(int argc, const char* argv[]);

} // namespace parser

#endif // LIBNOSTL_TESTS_CMDLINE_PARSER_H
