#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PahoMqttCpp::paho-mqttpp3" for configuration "Release"
set_property(TARGET PahoMqttCpp::paho-mqttpp3 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(PahoMqttCpp::paho-mqttpp3 PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/paho-mqttpp3.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/paho-mqttpp3.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS PahoMqttCpp::paho-mqttpp3 )
list(APPEND _IMPORT_CHECK_FILES_FOR_PahoMqttCpp::paho-mqttpp3 "${_IMPORT_PREFIX}/lib/paho-mqttpp3.lib" "${_IMPORT_PREFIX}/bin/paho-mqttpp3.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
