set(NAME "core")

configure_file(common.hpp.in common.hpp @ONLY)

ucm_add_files(
  "test.cpp"
  # "allocation.cpp"
  "macros.cpp"
  "state.cpp"
  TO SOURCES)
  
ucm_add_target(
  NAME ${NAME}
  TYPE STATIC
  SOURCES ${SOURCES}
  UNITY CPP_PER_UNITY 20
  PCH_FILE "precompiled.hpp")

add_library("${PROJECT_NAMESPACE}::${NAME}" ALIAS ${NAME})

target_include_directories(${NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR})

target_link_libraries(${NAME} PRIVATE
  ${CMAKE_THREAD_LIBS_INIT}
  spdlog::spdlog)

target_set_warnings(${NAME}
  ENABLE ALL
  # AS_ERROR ALL
  DISABLE Annoying)

set_target_properties(${NAME} PROPERTIES
    CXX_STANDARD 17 
    CXX_STANDARD_REQUIRED YES 
    CXX_EXTENSIONS NO)

target_enable_lto(${NAME} optimized)
