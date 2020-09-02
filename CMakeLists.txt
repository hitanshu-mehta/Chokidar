cmake_minimum_required(VERSION 3.1.0)

# set project name
project(IDS)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/mongo-cxx-driver")

add_subdirectory(pybind11)
include_directories("${CMAKE_SOURCE_DIR}/mongo-cxx-driver/include/mongocxx/v_noabi/")
include_directories("${CMAKE_SOURCE_DIR}/mongo-cxx-driver/include/bsoncxx/v_noabi/")
link_directories("${CMAKE_SOURCE_DIR}/mongo-cxx-driver/lib/")


file(GLOB all_src "${CMAKE_SOURCE_DIR}/src/*/*.hpp" "${CMAKE_SOURCE_DIR}/src/ui/*.h" "${CMAKE_SOURCE_DIR}/src/*.cpp")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(PCAP REQUIRED)
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

add_executable(IDS ${all_src})

target_link_libraries(IDS mongocxx bsoncxx pybind11::embed ${PCAP_LIBRARY} Qt5::Widgets)