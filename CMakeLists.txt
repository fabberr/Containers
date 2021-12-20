cmake_minimum_required(VERSION 3.21.4)

# Set project name
project(NoSTL)

# Add subdirectories
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/nostl") 	# project: libnostl (library)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/main") 	# project: main (executable)
