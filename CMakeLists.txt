cmake_minimum_required (VERSION 2.8.1)



set(POJECT_NAME EBNFDataGenerator)
set(TARGET_NAME EBNFDataGenerator)


#project name
project (${POJECT_NAME})
# .cpp files locate
set(SOURCES main.cpp)
file (GLOB SOURCES ${SOURCES})


add_subdirectory(src)

add_executable       (${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME} DataGenerator)

set(CMAKE_CXX_FLAGS -std=c++11)
