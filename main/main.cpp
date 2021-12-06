/********** Headers **********/

// C++ library
#include <iostream>	// std::cout
#include <utility> 	// std::move
#include <string> 	// std::string
#include <vector> 	// std::vector

// libnostl
#include <nostl/vector.h> // nostl::vector

/********** Functions **********/

/**
 * Prints a nostl::vector along with information about it's memory footprint to 
 * the standard character output.
*/
template<typename T, size_t N>
void print_vec_stats(const nostl::vector<T, N>& vec) {
	
	// print vector
	std::cout << vec << std::endl;
	
	// print memory usage information
	std::printf(
		"  len=%d elements, capacity=%d elements, elem_size=%d bytes\n", 
		vec.len(), 
		vec.capacity(), 
		sizeof (T)
	);
	std::printf(
		"  mem_usage=%d bytes, total_allocated_mem=%d bytes, unused_mem=%d bytes\n", 
		vec.memsize(), 
		vec.allocsize(), 
		vec.allocsize() - vec.memsize()
	);
}

/********** Main Entry Point **********/

int main() {

	// testing iterators
	nostl::vector<std::string> strVec;
	strVec += "vector";
	strVec += "of";
	strVec += "strings";
	strVec += "iwtcits";

	// range-based for loop
	std::cout << "range-based for loop:" << std::endl;
	for (const auto& e : strVec) {
		std::cout << e << std::endl;
	}

	// iterator
	std::cout << "\niterator:" << std::endl;
	nostl::vector<std::string>::iterator it;
	for (it = strVec.begin(); it != strVec.end(); it++) {
		std::cout << *it << std::endl;
	}

	// reverse iterator
	// std::cout << "\nreverse iterator:" << std::endl;
	// nostl::vector<std::string>::reverse_iterator rev;
	// for (rev = strVec.rbegin(); rev != strVec.rend(); rev++) {
	// 	std::cout << *rev << std::endl;
	// }

	// testing push_back calls from operator+= overloads
	nostl::vector<int> v1;
	int a = 2;
	int b = 3;
	v1 += 1;
	v1 += a;
	v1 += std::move(b);
	std::cout << '\n' << "v1: " << v1 << std::endl;

	// testing copy and move constructors
	nostl::vector<int> v2 = v1;
	// nostl::vector<int> v2 = std::move(v1);
	std::cout << '\n' << "v1: " << v1 << std::endl;
	std::cout << "v2: " << v2 << std::endl;

	// building new instance for testing copy and move assignment operator= overloads
	nostl::vector<int> v3;
	v3 += 4;
	v3 += 6;
	v3 += 7;
	v3 += 8;
	v3 += 9;
	std::cout << '\n' << "v3: " << v3 << std::endl;

	v2 = v3;
	v3 = std::move(v2);
	std::cout << '\n' << "v2: " << v2 << std::endl;
	std::cout << "v3: " << v3 << std::endl;
		
	// testing emplace_back
	nostl::vector<std::string> strings;
	strings.emplace_back("test");
	strings.emplace_back("string #2");
	strings.emplace_back("string #3, causes reallocation (used to crash on vector::resize call)");
	std::cout << '\n' << "strings: " << strings << std::endl;

	// testing copy constructor and copy assignment operator on std::string
	nostl::vector<std::string> s2 = strings;
	s2.emplace_back("this string belongs to s2 (a copy of strings");
	std::cout << '\n' << "strings: " << strings << std::endl;
	std::cout << "s2: " << s2 << std::endl;

	nostl::vector<std::string> s3;
	s3 = s2;
	s3.emplace_back("s3 is a copy of s2 (a copy of strings)");
	std::cout << '\n' << "s2: " << s2 << std::endl;
	std::cout << "s3: " << s3 << std::endl;

	// testing move constructor and copy assignment operator on std::string
	nostl::vector<std::string> s4 = std::move(strings);
	s4.emplace_back("this string belongs to s4 (moved from strings");
	std::cout << '\n' << "strings: " << strings << std::endl;
	std::cout << "s4: " << s4 << std::endl;

	nostl::vector<std::string> s5;
	s5 = std::move(s2);
	s5.emplace_back("s5 moved from s4 (moved from strings)");
	std::cout << '\n' << "s4: " << s2 << std::endl;
	std::cout << "s5: " << s5 << std::endl;

	return 0;
}

/** @todo test const iterators */
/** @todo test vector::resize */
