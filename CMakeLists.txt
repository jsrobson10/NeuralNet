cmake_minimum_required(VERSION 3.14)

project(NeuralNet VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-gdwarf-4")

file(GLOB_RECURSE SOURCES src/*.c*)
add_executable(NeuralNet ${SOURCES})

target_link_libraries(NeuralNet PUBLIC crypto glfw OpenGL m stdc++)

