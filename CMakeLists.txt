cmake_minimum_required (VERSION 2.8.1)


set(POJECT_NAME EBNFDataGenerator)
set(TARGET_NAME EBNFDataGenerator)


#project name
project (${POJECT_NAME})


add_subdirectory(src)

add_executable       (${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} DataGenerator)
