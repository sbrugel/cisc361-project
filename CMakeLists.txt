cmake_minimum_required(VERSION 3.19)
project(CISC361-Term-Project)
set(CMAKE_CXX_STANDARD 17)

# Compile with PIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Make executable
add_executable(${PROJECT_NAME}
        # Put files you want to build (.h, .cpp) in this list using this format
        # No semicolons, just newlines

        ${CMAKE_CURRENT_LIST_DIR}/src/utility/String.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/utility/String.h

        ${CMAKE_CURRENT_LIST_DIR}/src/Commands.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Commands.h
        ${CMAKE_CURRENT_LIST_DIR}/src/Job.h

        ${CMAKE_CURRENT_LIST_DIR}/src/Main.cpp
        src/HoldQueue.cpp src/HoldQueue.h)

# Copy necessary files
add_custom_target(${PROJECT_NAME}-Copy-Resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/inputs ${CMAKE_BINARY_DIR}/inputs)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-Copy-Resources)
