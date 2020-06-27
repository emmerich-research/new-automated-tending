project(device)

ucm_add_files(
  "init.cpp"
  "gpio.cpp"

  "identifier.cpp"

  # analog devices
  "analog.cpp"
  "PCF8591.cpp"

  # digital
  "pwm.cpp"

  # stepper
  "stepper.cpp"

  # shift register
  "shift_register.cpp"
  TO SOURCES)

ucm_add_target(
  NAME device
  TYPE STATIC
  SOURCES ${SOURCES}
  UNITY CPP_PER_UNITY 20
  PCH_FILE "device.hpp")

add_library("${PROJECT_NAMESPACE}::device" ALIAS device)

target_include_directories(device PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(device PUBLIC
  ${CMAKE_THREAD_LIBS_INIT}
  ${RASPI_LIB}
  "${PROJECT_NAMESPACE}::util"
  "${PROJECT_NAMESPACE}::core"
  "${PROJECT_NAMESPACE}::algo")

target_set_warnings(device
  ENABLE ALL
  # AS_ERROR ALL
  DISABLE Annoying)

set_target_properties(device PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

target_enable_lto(device optimized)
