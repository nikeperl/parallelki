cmake_minimum_required(VERSION 3.10)
project(SinArraySum)

option(USE_FLOAT "Use float instead of double" OFF)

if(USE_FLOAT)
    add_compile_definitions(TYPE=float)
else()
    add_compile_definitions(TYPE=double)
endif()

add_executable(sin_array_sum src/main.cpp)