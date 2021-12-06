cmake_minimum_required(VERSION 3.21.4)

# Set project name
project(NoSTL)

# Add library and main project subdirectories
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/nostl")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/main")
