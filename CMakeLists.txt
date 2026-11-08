cmake_minimum_required(VERSION 3.20)

project(exchange_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20) # compile as C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(exchange_sim src/main.cpp)
target_compile_options(exchange_sim PRIVATE -Wall -Wextra -Wpedantic)

add_executable(tests tests/test_parse.cpp src/parse.cpp)
target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic)
