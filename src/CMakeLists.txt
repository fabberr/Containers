#
# Top-level (root) CMakeLists.txt
#

# Set minimum CMake version required
cmake_minimum_required(VERSION 3.21.4)

# Set CMake project name
project(Containers)

# include CTest
include(CTest)

# Add subdirectories
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/nostl") 	# target: nostl (static library)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/demo") 	# target: Demo (executable)

# Check if in root CMakeLists.txt
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests") 	# target: UnitTests (executable)
endif ()

# Enable CTest
enable_testing()
