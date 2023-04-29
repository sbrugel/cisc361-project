cmake_minimum_required(VERSION 3.19)
project(CISC361-Term-Project)
set(CMAKE_CXX_STANDARD 20)

# Compile with PIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add files
list(
        APPEND ${PROJECT_NAME}_SOURCES

        # Put files you want to build (.h, .cpp) in this list using this format
        # No semicolons, just newlines

        ${CMAKE_CURRENT_LIST_DIR}/src/utility/String.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/utility/String.h

        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
)

# Make executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} src/Job.h)
