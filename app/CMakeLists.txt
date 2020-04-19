set(NAME "app")

project(${NAME})

ucm_add_files(
  "main.cpp"
  TO SOURCES)
  
ucm_add_target(
  NAME ${NAME}
  TYPE EXECUTABLE
  SOURCES ${SOURCES})
  # UNITY CPP_PER_UNITY 20
  # PCH_FILE "precompiled.hpp")

# target_include_directories(${NAME} PRIVATE
#   # ${PROJECT_SOURCE_DIR}
#   ${CMAKE_BINARY_DIR})

target_link_libraries(${NAME} PRIVATE
  "${PROJECT_NAMESPACE}::core")

target_set_warnings(${NAME}
  ENABLE ALL
  # AS_ERROR ALL
  DISABLE Annoying)

set_target_properties(${NAME} PROPERTIES
    CXX_STANDARD 17 
    CXX_STANDARD_REQUIRED YES 
    CXX_EXTENSIONS NO)

target_enable_lto(${NAME} optimized)
