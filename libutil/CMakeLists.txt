project(util)

ucm_add_files(
  "macros.cpp"
  "timer.cpp"
  TO SOURCES)
  
ucm_add_target(
  NAME util
  TYPE STATIC
  SOURCES ${SOURCES}
  UNITY CPP_PER_UNITY 20
  PCH_FILE "util.hpp")

add_library("${PROJECT_NAMESPACE}::util" ALIAS util)

target_include_directories(util PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(util PUBLIC
  ${CMAKE_THREAD_LIBS_INIT}
  fs::fs)

target_set_warnings(util
  ENABLE ALL
  # AS_ERROR ALL
  DISABLE Annoying)

set_target_properties(util PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES 
    CXX_EXTENSIONS NO)

target_enable_lto(util optimized)
