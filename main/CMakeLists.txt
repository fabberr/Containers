# Create executable target
add_executable(main ./src/main.cpp)

# Set the target's C++ standard
set_property(TARGET main PROPERTY CXX_STANDARD 17)

# Link libraries to target executable
target_link_libraries(main PRIVATE nostl)
