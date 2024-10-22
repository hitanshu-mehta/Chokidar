# Copyright 2017 MongoDB Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

message(WARNING "This CMake target is deprecated.  Use 'mongo::bsoncxx_static' instead.  Consult the example projects for further details.")

set(LIBBSONCXX_STATIC_VERSION_MAJOR 3)
set(LIBBSONCXX_STATIC_VERSION_MINOR 5)
set(LIBBSONCXX_STATIC_VERSION_PATCH 1)
set(LIBBSONCXX_STATIC_PACKAGE_VERSION 3.5.1)

# We need to pull in the libbson-static-* library to read the BSON_STATIC_LIBRARIES variable.  We
# can ignore the other variables exported by that package (e.g. BSON_STATIC_INCLUDE_DIRS,
# BSON_STATIC_DEFINITIONS), since bsoncxx hides the existence of libbson from the user through
# abstraction.  bsoncxx users generally should not need to include libbson headers directly.
find_package(libbson-static-1.0 1.13.0 REQUIRED)


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was libbsoncxx-static-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

set_and_check(LIBBSONCXX_STATIC_INCLUDE_DIRS "${PACKAGE_PREFIX_DIR}/include/bsoncxx/v_noabi")

# We want to provide an absolute path to the library and we know the directory and the base name,
# but not the suffix, so we use CMake's find_library() to pick that up.
find_library(LIBBSONCXX_STATIC_LIBRARY_PATH bsoncxx-static PATHS "${PACKAGE_PREFIX_DIR}/lib" NO_DEFAULT_PATH)
set(LIBBSONCXX_STATIC_LIBRARIES ${LIBBSONCXX_STATIC_LIBRARY_PATH} ${BSON_STATIC_LIBRARIES})

set(LIBBSONCXX_STATIC_DEFINITIONS BSONCXX_STATIC)
