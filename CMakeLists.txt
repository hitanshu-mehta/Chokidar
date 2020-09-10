cmake_minimum_required(VERSION 3.1.0)

# set project name
project(IDS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
set(CPACK_PACKAGE_VERSION_MAJOR "${IDS_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${IDS_VERSION_MINOR}")


# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/mongo-cxx-driver)
set(SRC_PATH ${CMAKE_SOURCE_DIR}/src)
set(UI_PATH ${SRC_PATH}/ui)
set(WORKER_PATH ${SRC_PATH}/worker)
set(GRAPH_PATH ${SRC_PATH}/graph)


add_subdirectory(pybind11)
include_directories(${CMAKE_SOURCE_DIR}/mongo-cxx-driver/include/mongocxx/v_noabi/)
include_directories(${CMAKE_SOURCE_DIR}/mongo-cxx-driver/include/bsoncxx/v_noabi/)
link_directories(${CMAKE_SOURCE_DIR}/mongo-cxx-driver/lib/)



list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
# set(ENV{PYTHONPATH} ${CMAKE_SOURCE_DIR}/model/inference.py ${CMAKE_SOURCE_DIR}/model/fetch_data.py) 
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui Charts)
find_package(pybind11 CONFIG REQUIRED)
find_package(PCAP REQUIRED)
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)



set(project_source
    ${SRC_PATH}/main.cpp
    ${SRC_PATH}/mainwindow.cpp
    ${SRC_PATH}/packet_capture.cpp
    ${SRC_PATH}/packet_capture_engine.cpp
    ${SRC_PATH}/packet_parser.cpp
    ${SRC_PATH}/basic_flow.cpp
    ${SRC_PATH}/basic_packet_info.cpp
    ${SRC_PATH}/flow_generator.cpp
    ${SRC_PATH}/flow_generator_engine.cpp
    ${SRC_PATH}/database.cpp
    ${SRC_PATH}/session_stats.cpp
    ${WORKER_PATH}/packet_capture_worker.cpp
    ${WORKER_PATH}/ui_reload_worker.cpp
    ${WORKER_PATH}/inference_worker.cpp
    ${SRC_PATH}/Chart.cpp
    )

set(project_headers
    ${SRC_PATH}/packet_capture.hpp
    ${SRC_PATH}/packet_capture_engine.hpp
    ${SRC_PATH}/packet_parser.hpp
    ${SRC_PATH}/basic_flow.hpp
    ${SRC_PATH}/basic_packet_info.hpp
    ${SRC_PATH}/flow_features.hpp
    ${SRC_PATH}/flow_generator.hpp
    ${SRC_PATH}/flow_generator_engine.hpp
    ${SRC_PATH}/constants.hpp
    ${SRC_PATH}/ether_header.hpp
    ${SRC_PATH}/ip_header.hpp
    ${SRC_PATH}/protocols.hpp
    ${SRC_PATH}/tcp_header.hpp
    ${SRC_PATH}/udp_header.hpp
    ${SRC_PATH}/utils.hpp
    ${SRC_PATH}/session_stats.hpp
    ${SRC_PATH}/mainwindow.h
    ${WORKER_PATH}/packet_capture_worker.h
    ${WORKER_PATH}/callback_timer.hpp
    ${WORKER_PATH}/ui_reload_worker.hpp
    ${WORKER_PATH}/inference_worker.hpp
    ${SRC_PATH}/Chart.h
)

set (project_headers_mac ${SRC_PATH}/mainwindow.h ${WORKER_PATH}/packet_capture_worker.h ${WORKER_PATH}/ui_reload_worker.hpp ${SRC_PATH}/Chart.h)

set(project_ui ${UI_PATH}/mainwindow.ui)

qt5_wrap_ui(project_headers_wrapped ${project_ui})
qt5_wrap_cpp(project_source_mac ${project_headers_mac})

add_executable(${PROJECT_NAME} ${project_source} ${project_headers} ${project_headers_wrapped} ${project_source_mac})

target_link_libraries(${PROJECT_NAME} ${PCAP_LIBRARY} mongocxx bsoncxx pybind11::embed Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Charts)
