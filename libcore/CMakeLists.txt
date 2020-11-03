project(core)

configure_file(common.hpp.in common.hpp @ONLY)
configure_file(common.cpp.in common.cpp @ONLY)

ucm_add_files(
  "${CMAKE_CURRENT_BINARY_DIR}/common.cpp"
  "init.cpp"
  "config.cpp"
  "logger.cpp"
  "state.cpp"
  "listener.cpp"
  TO SOURCES)
  
ucm_add_target(
  NAME core
  TYPE STATIC
  SOURCES ${SOURCES}
  UNITY CPP_PER_UNITY 20
  PCH_FILE "core.hpp")

add_library("${PROJECT_NAMESPACE}::core" ALIAS core)

target_include_directories(core PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(core PUBLIC
  ${CMAKE_THREAD_LIBS_INIT}
  fs::fs
  spdlog::spdlog
  toml11::toml11
  date::date
  "${PROJECT_NAMESPACE}::util")

target_set_warnings(core
  ENABLE ALL
  # AS_ERROR ALL
  DISABLE Annoying)

set_target_properties(core PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES 
    CXX_EXTENSIONS NO)

target_enable_lto(core optimized)
