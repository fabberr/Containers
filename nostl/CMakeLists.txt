# Create library target
add_library(
	nostl STATIC
	./include/nostl/arr_iterators.h
	./include/nostl/vector.h
	./include/nostl/array.h
)

# Set the target's language
set_property(TARGET nostl PROPERTY LINKER_LANGUAGE CXX) # language: C++

# Set the target's C++ standard
set_property(TARGET nostl PROPERTY CXX_STANDARD 20) # -std=c++20

# Define target's include directories
target_include_directories(
	nostl 
	PUBLIC ./include
)
