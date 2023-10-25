cmake_minimum_required(VERSION 3.4.1)

project(QParse)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT COMMAND testBuilder_build)
    add_subdirectory(testBuilder)
    testBuilder_set_current_working_directory_to_default_binary_directory()
endif()

testBuilder_add_include(QParse ${CMAKE_CURRENT_SOURCE_DIR})
testBuilder_add_source(QParse Conditional.cpp)
testBuilder_add_source(QParse Error.cpp)
testBuilder_add_source(QParse Input.cpp)
testBuilder_add_source(QParse Iterator.cpp)
testBuilder_add_source(QParse IteratorMatcher.cpp)
testBuilder_add_source(QParse Rules.cpp)
testBuilder_add_source(QParse Stack.cpp)
testBuilder_build_shared_library(QParse)

set(QPARSE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)