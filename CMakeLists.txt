# Central cmake list to add stuff

cmake_minimum_required(VERSION 3.5)
project(glPlayground
  LANGUAGES CXX
  VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

add_compile_options(-Wall -Wextra -Wpedantic)

add_subdirectory(triangleLegacy)
add_subdirectory(triangleModern)

