project(app)

ucm_add_files(
  "action.cpp"
  "event.cpp"
  "guard.cpp"
  "machine.cpp"

  "main.cpp"
  TO SOURCES)
  
ucm_add_target(
  NAME app
  TYPE EXECUTABLE
  SOURCES ${SOURCES}
  UNITY CPP_PER_UNITY 20
  PCH_FILE "precompiled.hpp")

target_include_directories(app PUBLIC
  "$<BUILD_INTERFACE:${AFSM_INCLUDE_DIRS}>")

target_link_libraries(app PRIVATE
  "${PROJECT_NAMESPACE}::core"
  "${PROJECT_NAMESPACE}::algo"
  "${PROJECT_NAMESPACE}::device"
  "${PROJECT_NAMESPACE}::mechanism")

target_set_warnings(app
  ENABLE ALL
  # AS_ERROR ALL
  DISABLE Annoying)

set_target_properties(app PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES 
    CXX_EXTENSIONS NO)

target_enable_lto(app optimized)
