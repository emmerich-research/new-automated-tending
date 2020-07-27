project(mechanism)

set(SYNC_DRIVER OFF)

ucm_add_files(
  "init.cpp"
  "movement.cpp"
  "liquid-refilling.cpp"
  TO SOURCES)

ucm_add_target(
  NAME mechanism
  TYPE STATIC
  SOURCES ${SOURCES}
  UNITY CPP_PER_UNITY 20
  PCH_FILE "mechanism.hpp")

add_library("${PROJECT_NAMESPACE}::mechanism" ALIAS mechanism)

target_include_directories(mechanism PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(mechanism PUBLIC
  ${CMAKE_THREAD_LIBS_INIT}
  ${RASPI_LIB}
  "${PROJECT_NAMESPACE}::util"
  "${PROJECT_NAMESPACE}::core"
  "${PROJECT_NAMESPACE}::algo"
  "${PROJECT_NAMESPACE}::device")

if (SYNC_DRIVER)
  target_compile_definitions(mechanism PUBLIC
    SYNC_DRIVER)
endif()

target_set_warnings(mechanism
  ENABLE ALL
  # AS_ERROR ALL
  DISABLE Annoying)

set_target_properties(mechanism PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

target_enable_lto(mechanism optimized)
