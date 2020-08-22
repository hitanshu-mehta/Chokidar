include(CMakeFindDependencyMacro)

find_dependency(bsoncxx REQUIRED)
include("${CMAKE_CURRENT_LIST_DIR}/mongocxx_targets.cmake")
