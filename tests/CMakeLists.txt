cmake_minimum_required (VERSION 2.8.1)

set(TARGET_NAME DataGeneratorTests)
set(POJECT_NAME DataGeneratorTests)


#project name
project (${POJECT_NAME})

# .cpp files locale
set(SOURCES *.cpp)

# .h files locale
set(INCLUDE_DIRS ../src $ENV{THRDPARTY}/include)

# .libraries
#add_library(gtest STATIC IMPORTED)
#set_property(TARGET gtest PROPERTY IMPORTED_LOCATION $ENV{THRDPARTY}/lib/libgtest.a)

#add_library(gtest_main STATIC IMPORTED)
#set_property(TARGET gtest_main PROPERTY IMPORTED_LOCATION $ENV{THRDPARTY}/lib/libgtest_main.a)
# generator

set(GTEST_ROOT  $ENV{THRDPARTY}/gtest)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})




include_directories (${INCLUDE_DIRS})
file (GLOB SOURCES ${SOURCES})
add_subdirectory(../src build)

add_executable       (${TARGET_NAME} ${SOURCES})
#target_link_libraries(${TARGET_NAME} gtest)
#target_link_libraries(${TARGET_NAME} gtest_main)
target_link_libraries(${TARGET_NAME} ${GTEST_BOTH_LIBRARIES})
target_link_libraries(${TARGET_NAME} DataGenerator)

# google test require -pthread
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

