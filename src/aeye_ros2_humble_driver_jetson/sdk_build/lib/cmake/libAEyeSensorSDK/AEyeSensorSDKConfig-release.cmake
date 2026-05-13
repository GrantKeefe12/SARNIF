#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "AEyeSensorSDK" for configuration "Release"
set_property(TARGET AEyeSensorSDK APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(AEyeSensorSDK PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libAEyeSensorSDK.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS AEyeSensorSDK )
list(APPEND _IMPORT_CHECK_FILES_FOR_AEyeSensorSDK "${_IMPORT_PREFIX}/lib/libAEyeSensorSDK.a" )

# Import target "AEyeSensorSDKShared" for configuration "Release"
set_property(TARGET AEyeSensorSDKShared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(AEyeSensorSDKShared PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libAEyeSensorSDKShared.so"
  IMPORTED_SONAME_RELEASE "libAEyeSensorSDKShared.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS AEyeSensorSDKShared )
list(APPEND _IMPORT_CHECK_FILES_FOR_AEyeSensorSDKShared "${_IMPORT_PREFIX}/lib/libAEyeSensorSDKShared.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
