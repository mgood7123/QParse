cmake_minimum_required(VERSION 3.4.1)

project(UltraParse)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT COMMAND testBuilder_build)
    add_subdirectory(testBuilder)
    testBuilder_set_current_working_directory_to_default_binary_directory()
endif()

add_subdirectory(QParse)

add_subdirectory(MiniDoc)
add_subdirectory(ManagedObject)

testBuilder_add_include(UltraParse include)
testBuilder_add_source(UltraParse src/empty.cpp)
testBuilder_add_library(UltraParse minidoc)
testBuilder_add_library(UltraParse managed_object)
testBuilder_build(UltraParse EXECUTABLES)

set(ULTRAPARSE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
