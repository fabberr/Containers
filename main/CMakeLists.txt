# Create executable target
add_executable(${PROJECT_NAME} main.cpp)

# Set the target's C++ standard
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Link libraries to target executable
target_link_libraries(${PROJECT_NAME} PRIVATE nostl)
