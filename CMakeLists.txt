cmake_minimum_required(VERSION 3.21.4)

# Set project name
project(Containers)

# Add subdirectories
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/nostl") 	# target: nostl (library)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/demo") 	# target: demo (executable)
