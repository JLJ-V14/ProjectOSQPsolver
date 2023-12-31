#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "osqp::osqpstatic" for configuration "RelWithDebInfo"
set_property(TARGET osqp::osqpstatic APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(osqp::osqpstatic PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "C"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/osqp.lib"
  )

list(APPEND _cmake_import_check_targets osqp::osqpstatic )
list(APPEND _cmake_import_check_files_for_osqp::osqpstatic "${_IMPORT_PREFIX}/lib/osqp.lib" )

# Import target "osqp::osqp" for configuration "RelWithDebInfo"
set_property(TARGET osqp::osqp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(osqp::osqp PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/osqp.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/osqp.dll"
  )

list(APPEND _cmake_import_check_targets osqp::osqp )
list(APPEND _cmake_import_check_files_for_osqp::osqp "${_IMPORT_PREFIX}/lib/osqp.lib" "${_IMPORT_PREFIX}/bin/osqp.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
