cmake_minimum_required(VERSION 3.14)

project(w3d_reader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("include")

file(GLOB SRC_DIR "src/*.cpp")
file(GLOB INCLUDE_DIR "include/*.h" "include/*/*.hpp" "include/*/*.h")
add_library(w3d_reader ${SRC_DIR} ${INCLUDE_DIR})
