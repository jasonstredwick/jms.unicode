# Copyright © 2024 Nemanja Trifunovic

project(
  jcu_tests
  VERSION 0.1
  DESCRIPTION "Tests for the jcu project."
  LANGUAGES CXX)


include(FetchContent)

FetchContent_Declare(
  ftest

  GIT_REPOSITORY https://github.com/nemtrif/ftest.git
  GIT_TAG        0.3)

FetchContent_MakeAvailable(ftest)

include(CTest)
add_library (ftest INTERFACE)
target_include_directories(ftest INTERFACE ${ftest_SOURCE_DIR})

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(utf_coretest utf/core.test.cpp)
target_include_directories(utf_coretest PRIVATE ${PROJECT_SOURCE_DIR}/../include)
target_link_libraries(utf_coretest PRIVATE ftest)
set_target_properties(utf_coretest PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
add_test(utf_coretest utf_coretest)

add_executable(utf_decodeencodetest utf/decode_encode.test.cpp)
target_include_directories(utf_decodeencodetest PRIVATE ${PROJECT_SOURCE_DIR}/../include)
target_link_libraries(utf_decodeencodetest PRIVATE ftest)
set_target_properties(utf_decodeencodetest PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
add_test(utf_decodeencodetest utf_decodeencodetest)

add_executable(utf_formattest utf/format.test.cpp)
target_include_directories(utf_formattest PRIVATE ${PROJECT_SOURCE_DIR}/../include)
target_link_libraries(utf_formattest PRIVATE ftest)
set_target_properties(utf_formattest PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
add_test(utf_formattest utf_formattest)


add_executable(utf_iteratorstest utf/iterators.test.cpp)
target_include_directories(utf_iteratorstest PRIVATE ${PROJECT_SOURCE_DIR}/../include)
target_link_libraries(utf_iteratorstest PRIVATE ftest)
set_target_properties(utf_iteratorstest PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
add_test(utf_iteratorstest utf_iteratorstest)

add_executable(utf_viewstest utf/views.test.cpp)
target_include_directories(utf_viewstest PRIVATE ${PROJECT_SOURCE_DIR}/../include)
target_link_libraries(utf_viewstest PRIVATE ftest)
set_target_properties(utf_viewstest PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
add_test(utf_viewstest utf_viewstest)

add_executable(utf_utilitytest utf/utility.test.cpp)
target_include_directories(utf_utilitytest PRIVATE ${PROJECT_SOURCE_DIR}/../include)
target_link_libraries(utf_utilitytest PRIVATE ftest)
set_target_properties(utf_utilitytest PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
add_test(utf_utilitytest utf_utilitytest)

#add_executable(bidi_basictest bidi/basic.test.cpp)
#target_include_directories(bidi_basictest PRIVATE ${PROJECT_SOURCE_DIR}/../include)
#target_link_libraries(bidi_basictest PRIVATE ftest)
#set_target_properties(bidi_basictest PROPERTIES
#    CXX_STANDARD 23
#    CXX_STANDARD_REQUIRED YES
#    CXX_EXTENSIONS NO
#)
#add_test(bidi_basictest bidi_basictest)
