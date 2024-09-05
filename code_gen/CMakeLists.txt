# Copyright © 2024 Jason Stredwick

project(
  jcu_code_gen
  VERSION 0.1
  DESCRIPTION "Generate source code for the jcu project."
  LANGUAGES CXX)


add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(code_gen main.cpp)
target_include_directories(code_gen PRIVATE ${PROJECT_SOURCE_DIR}/../include)
target_include_directories(code_gen PRIVATE ${PROJECT_SOURCE_DIR}/code_gen)
set_target_properties(code_gen PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
