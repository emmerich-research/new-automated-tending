project(driver)

ucm_add_files(
  "stepper.cpp"
  TO SOURCES)
  
ucm_add_target(
  NAME driver
  TYPE EXECUTABLE
  SOURCES ${SOURCES}
  # UNITY CPP_PER_UNITY 20
  PCH_FILE "precompiled.hpp")

# target_include_directories(${NAME} PRIVATE
#   # ${PROJECT_SOURCE_DIR}
#   ${CMAKE_BINARY_DIR})

target_link_libraries(driver PRIVATE
  "${PROJECT_NAMESPACE}::core"
  "${PROJECT_NAMESPACE}::algo"
  "${PROJECT_NAMESPACE}::device"
  "${PROJECT_NAMESPACE}::mechanism")

target_set_warnings(driver
  ENABLE ALL
  # AS_ERROR ALL
  DISABLE Annoying)

set_target_properties(driver PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES 
    CXX_EXTENSIONS NO)

target_enable_lto(driver optimized)
