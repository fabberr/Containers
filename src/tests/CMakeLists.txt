# Set target's name
project(UnitTests)

# Create executable target
add_executable(
	${PROJECT_NAME} 
	./src/main.cpp
	./src/cmdline_parser.cpp 
	./src/unit_tests.cpp
	./src/util.cpp
)

# Setting the target's properties
set_target_properties(
	${PROJECT_NAME} PROPERTIES 	# Setting properties for ´UnitTests´ target
	CXX_STANDARD 20				# Set the target's C++ standard to C++ 20
)

# Link libraries to target executable
target_link_libraries(${PROJECT_NAME} PRIVATE nostl) # link libnostl.a

#---------- Unit Tests ----------

# Tests constructors and assignment operations.
add_test(
	NAME "nostl::vector: Constructors and assignment operations." 
	COMMAND ${PROJECT_NAME} vector initialization
)

# Tests constructor and copy assignment operation from std::vector.
add_test(
	NAME "nostl::vector: Copy constructor and copy assignment from std::vector." 
	COMMAND ${PROJECT_NAME} vector initialization-std
)

# Tests equality and inequality operators.
add_test(
	NAME "nostl::vector: Equality and inequality operators." 
	COMMAND ${PROJECT_NAME} vector compare
)
