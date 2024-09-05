# Copyright © 2024 Nemanja Trifunovic

cmake_minimum_required(VERSION 3.16)

project(
  jcu
  VERSION 0.1
  DESCRIPTION "A C++ library for a subset of unicode functionality."
  LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)

enable_testing()
add_subdirectory(tests EXCLUDE_FROM_ALL)
add_subdirectory(code_gen EXCLUDE_FROM_ALL)
