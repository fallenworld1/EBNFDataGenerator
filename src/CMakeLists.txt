#minimum cmake version to build
cmake_minimum_required (VERSION 2.8.1)

set(TARGET_NAME DataGenerator)
set(POJECT_NAME DataGenerator)



# .cpp files locate
set(SOURCES *.cpp)

#project name
project (${POJECT_NAME})
# .h files locate
include_directories (${INCLUDE_DIRS})


link_directories(${LINK_DIRS})

# adding all .cpp files to SOURCES variable
file (GLOB SOURCES ${SOURCES})

add_library(${TARGET_NAME} STATIC ${SOURCES})

#gnu require explicit flag c++11, msvc not
if(${CMAKE_COMPILER_IS_GNUCXX})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
