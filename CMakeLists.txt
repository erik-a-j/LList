cmake_minimum_required(VERSION 3.20)
project(llist LANGUAGES CXX)

add_compile_options(-Wall -Wextra -pedantic)
add_executable(${PROJECT_NAME} main.cpp)