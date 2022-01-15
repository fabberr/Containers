# Set target's name
project(demo)

# Create executable target
add_executable(${PROJECT_NAME} ./src/demo.cpp)

# Setting the target's properties
set_target_properties(
	${PROJECT_NAME} PROPERTIES # Setting properties for ´demo´ target
	CXX_STANDARD 20	# Set the target's C++ standard to C++ 20
)

# Link libraries to target executable
target_link_libraries(${PROJECT_NAME} PRIVATE nostl) # link libnostl.a
