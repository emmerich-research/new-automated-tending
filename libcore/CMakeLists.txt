set(NAME "core")

project(${NAME})

configure_file(common.hpp.in common.hpp @ONLY)

ucm_add_files(
  "init.cpp"
  "macros.cpp"
  "logger.cpp"
  "state.cpp"
  "config.cpp"
  TO SOURCES)
  
ucm_add_target(
  NAME ${NAME}
  TYPE STATIC
  SOURCES ${SOURCES}
  UNITY CPP_PER_UNITY 20
  PCH_FILE "core.hpp")

add_library("${PROJECT_NAMESPACE}::${NAME}" ALIAS ${NAME})

target_include_directories(${NAME} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${NAME} PUBLIC
  ${CMAKE_THREAD_LIBS_INIT}
  fs::fs
  spdlog::spdlog
  yaml::yaml)

target_set_warnings(${NAME}
  ENABLE ALL
  # AS_ERROR ALL
  DISABLE Annoying)

set_target_properties(${NAME} PROPERTIES
    CXX_STANDARD 17 
    CXX_STANDARD_REQUIRED YES 
    CXX_EXTENSIONS NO)

target_enable_lto(${NAME} optimized)

