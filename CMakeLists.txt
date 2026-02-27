cmake_minimum_required(VERSION 3.14)
project(CppChat VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find dependencies
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(SQLite3 REQUIRED)
find_package(nlohmann_json REQUIRED)

# Include directories
include_directories(${Boost_INCLUDE_DIRS} ${SQLite3_INCLUDE_DIRS})

# Add subdirectories
add_subdirectory(src)

# Testing
enable_testing()
add_subdirectory(tests)
