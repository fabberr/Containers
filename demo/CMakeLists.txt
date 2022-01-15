# Create executable target
add_executable(demo ./src/demo.cpp)

# Setting the target's properties
set_target_properties(
	demo PROPERTIES # Setting properties for ´demo´ target
	CXX_STANDARD 20	# Set the target's C++ standard to C++ 20
)

# Link libraries to target executable
target_link_libraries(demo PRIVATE nostl)
