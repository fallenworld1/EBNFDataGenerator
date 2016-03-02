#minimu cmake version to build
cmake_minimum_required (VERSION 2.8.1)

set(TARGET_NAME DataGenerator)
set(POJECT_NAME EBNFDataGenerator)
set(CMAKE_CXX_FLAGS -std=c++11)


set(STATIC_LIBS libboost_chrono-mgw49-mt-sd-1_60.a
                libboost_system-mgw49-mt-sd-1_60.a
                libboost_thread-mgw49-mt-sd-1_60.a
)
set(BOOST_DIR "$ENV{THRDPARTY}/boost")
#find_package(BOOST_DIR REQUIRED chrono )
set(INCLUDE_DIRS src ${BOOST_DIR}/include)
set(LINK_DIRS  ${BOOST_DIR}/lib/chrono/build/gcc-mingw-4.9.2/debug/link-static/runtime-link-static/threading-multi
               ${BOOST_DIR}/lib/system/build/gcc-mingw-4.9.2/debug/link-static/runtime-link-static/threading-multi
               ${BOOST_DIR}/lib/thread/build/gcc-mingw-4.9.2/debug/link-static/runtime-link-static/threading-multi )
set(SOURCES src/*.cpp)

#project name
project (${POJECT_NAME})
# .h files locale
include_directories (${INCLUDE_DIRS})

#add_library(boost SHARED C://Projects/lib/boost/lib/chrono/build/gcc-mingw-4.9.2/debug)

link_directories(${LINK_DIRS})

# adding all .cpp files to SOURCES variable
file (GLOB SOURCES ${SOURCES})

add_executable (${TARGET_NAME} ${SOURCES})

#target_link_libraries(DataGenerator chrono/build/gcc-mingw-4.9.2/debug/link-static/runtime-link-static/libboost_chrono-mgw49-sd-1_60.a)
target_link_libraries(${TARGET_NAME} ${STATIC_LIBS})
