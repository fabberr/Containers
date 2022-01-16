#ifndef LIBNOSTL_UNIT_TESTS_H
#define LIBNOSTL_UNIT_TESTS_H

/********** Unit Tests (declaration) **********/

/** Provides unit tests for libnostl. */
namespace test {

	/** Subset of tests for nostl::vector. */
	namespace vector {
		int constructors_and_assignment_operations();
		int constructors_and_assignment_operations_std();
		int compare();
	} // namspace vector

	/** Subset of tests for nostl::array. */
	namespace array {
		// ...
	} // namspace array

} // namespace test

#endif // LIBNOSTL_UNIT_TESTS_H
