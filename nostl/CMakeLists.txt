# Create library target
add_library(
	nostl STATIC
	./include/nostl/arr_iterators.h
	./include/nostl/vector.h
	./include/nostl/array.h
)

# Set the target's language to C++
set_property(TARGET nostl PROPERTY LINKER_LANGUAGE CXX)

# Set the target's C++ standard
set_property(TARGET nostl PROPERTY CXX_STANDARD 17)

# Define target's include directories
target_include_directories(
	nostl 
	PUBLIC ./include
)
