#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "test_suite" for configuration "Debug"
set_property(TARGET test_suite APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(test_suite PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/test_suite-d.exe"
  )

list(APPEND _cmake_import_check_targets test_suite )
list(APPEND _cmake_import_check_files_for_test_suite "${_IMPORT_PREFIX}/bin/test_suite-d.exe" )

# Import target "catchtest" for configuration "Debug"
set_property(TARGET catchtest APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(catchtest PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/catchtest-d.exe"
  )

list(APPEND _cmake_import_check_targets catchtest )
list(APPEND _cmake_import_check_files_for_catchtest "${_IMPORT_PREFIX}/bin/catchtest-d.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
