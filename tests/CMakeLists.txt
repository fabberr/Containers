# Set target's name
project(UnitTests)

# Create executable target
add_executable(${PROJECT_NAME} ./src/main.cpp ./src/unit_tests.cpp)

# Setting the target's properties
set_target_properties(
	${PROJECT_NAME} PROPERTIES 	# Setting properties for ´tests´ target
	CXX_STANDARD 20				# Set the target's C++ standard to C++ 20
)

# Link libraries to target executable
target_link_libraries(${PROJECT_NAME} PRIVATE nostl) # link libnostl.a

#---------- Unit Tests ----------

# demo test
# add_test(
# 	NAME "demo test" 
# 	COMMAND ${PROJECT_NAME}
# )

# ./UnitTests --constructors
add_test(
	NAME "Constructors & Assignment Operations" 
	COMMAND ${PROJECT_NAME} --constructors
)
