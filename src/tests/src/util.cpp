/********** Included Headers **********/

// internal
#include "../include/util.h" // utility function declarations

/********** Utility Functions (definitions) **********/

/**
 * Prints the contents of a nostl::vector along with information about its 
 * memory footprint to the standard character output.
*/
template<typename T, size_t N>
void util::print_vec_stats(const nostl::vector<T, N>& vec) {
	
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

/**
 * Stream insertion operator overload for std::string specialization of 
 * std::vector.
*/
std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& rhs) {

	// iterator type alias
	using itr_t = typename std::vector<std::string>::const_iterator;

	// begin vector
	os << "[";

	// iterate through vector, inserting each string
	for (itr_t it = rhs.begin(); it != rhs.end(); ++it) {
		// insert currrent string, followed by a comma if not last element
		os << '\"' << *it << '\"';
		if ((it + 1) != rhs.end()) {
			os << ", ";
		}
	}

	// end vector
	os << "]";

	// return reference to output stream
	return os;
}
