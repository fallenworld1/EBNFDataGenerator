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
add_library(gtest STATIC IMPORTED)
set_property(TARGET gtest PROPERTY IMPORTED_LOCATION $ENV{THRDPARTY}/lib/gtest_64.lib)

add_library(gtest_main STATIC IMPORTED)
set_property(TARGET gtest_main PROPERTY IMPORTED_LOCATION $ENV{THRDPARTY}/lib/gtest_main_64.lib)
# generator



#find_package(gtest REQUIRED)
#find_package(gtest_main REQUIRED)
#include_directories(${GTEST_INCLUDE_DIR})
#include_directories(${GTEST_MAIN_INCLUDE_DIR})




include_directories (${INCLUDE_DIRS})
file (GLOB SOURCES ${SOURCES})
add_subdirectory(../src build)

add_executable       (${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME} gtest)
target_link_libraries(${TARGET_NAME} gtest_main)
target_link_libraries(${TARGET_NAME} DataGenerator)
