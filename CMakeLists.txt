# This is the top-level CMakeLists.txt file

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(H01_cpp_basics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# create static library
add_library(bookstore_lib
        src/author.cpp include/author.hpp
        src/book.cpp include/book.hpp
        src/book_store.cpp include/book_store.hpp)

target_include_directories(bookstore_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# executables
add_executable(main main.cpp)
target_link_libraries(main PRIVATE bookstore_lib)

# dependencies
add_subdirectory(contrib)

# tests
enable_testing()
add_subdirectory(tests)