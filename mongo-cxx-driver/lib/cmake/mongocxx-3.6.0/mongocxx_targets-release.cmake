#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mongo::mongocxx_shared" for configuration "Release"
set_property(TARGET mongo::mongocxx_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(mongo::mongocxx_shared PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "mongo::mongoc_shared"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libmongocxx.so.3.6.0"
  IMPORTED_SONAME_RELEASE "libmongocxx.so._noabi"
  )

list(APPEND _IMPORT_CHECK_TARGETS mongo::mongocxx_shared )
list(APPEND _IMPORT_CHECK_FILES_FOR_mongo::mongocxx_shared "${_IMPORT_PREFIX}/lib/libmongocxx.so.3.6.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
