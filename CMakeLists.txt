cmake_minimum_required(VERSION 3.10)

# set project name
project(IDS)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/mongo-cxx-driver")

include_directories("${CMAKE_SOURCE_DIR}/mongo-cxx-driver/include/mongocxx/v_noabi/")
include_directories("${CMAKE_SOURCE_DIR}/mongo-cxx-driver/include/bsoncxx/v_noabi/")
link_directories("${CMAKE_SOURCE_DIR}/mongo-cxx-driver/lib/")


file(GLOB all_src "${CMAKE_SOURCE_DIR}/src/*.hpp" "${CMAKE_SOURCE_DIR}/src/*.cpp")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(PCAP REQUIRED)
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

add_executable(IDS ${all_src})

target_link_libraries(IDS mongocxx bsoncxx ${PCAP_LIBRARY})