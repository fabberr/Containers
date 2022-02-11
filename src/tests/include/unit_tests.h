#ifndef LIBNOSTL_TESTS_UNIT_TESTS_H
#define LIBNOSTL_TESTS_UNIT_TESTS_H

/********** unit_tests.h **********/

/** Provides unit tests for libnostl. */
namespace test {

	/********** Unit Tests (declarations) **********/
	
	/** Subset of tests for nostl::vector. Specify `vector` as command line argument. */
	namespace vector {
		int initialization();
		int initialization_std();
		int compare();
	} // namespace vector

	/** Subset of tests for nostl::array. Specify `array` as command line argument. */
	namespace array {
		// ...
	} // namespace array

} // namespace test

#endif // LIBNOSTL_TESTS_UNIT_TESTS_H
