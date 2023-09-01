# from here
# http://google.github.io/googletest/quickstart-cmake.html

cmake_minimum_required(VERSION 3.14)
project(wptest_git)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(lib/googletest)