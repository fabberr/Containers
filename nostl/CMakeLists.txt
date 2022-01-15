# Set target's name
project(nostl)

# Create library target
add_library(
	${PROJECT_NAME} STATIC
	./include/nostl/arr_iterators.h
	./include/nostl/vector.h
	./include/nostl/array.h
)

# Setting the target's properties
set_target_properties(
	${PROJECT_NAME} PROPERTIES 		# Setting properties for `nostl` target
	LINKER_LANGUAGE CXX 	# Set the target's language to C++
	CXX_STANDARD 20 		# Set the target's C++ standard to C++ 20
)

# Define target's include directories
target_include_directories(
	${PROJECT_NAME} 
	PUBLIC ./include
)
