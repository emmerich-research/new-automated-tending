project(machine)

ucm_add_files(
  "action.cpp"
  "event.cpp"
  "guard.cpp"
  "state.cpp"

  TO SOURCES)
  
ucm_add_target(
  NAME machine
  TYPE STATIC
  SOURCES ${SOURCES}
  UNITY CPP_PER_UNITY 20
  PCH_FILE "machine.hpp")

add_library("${PROJECT_NAMESPACE}::machine" ALIAS machine)

target_include_directories(machine PUBLIC
  "$<BUILD_INTERFACE:${AFSM_INCLUDE_DIRS}>")

target_link_libraries(machine PRIVATE
  "${PROJECT_NAMESPACE}::util"
  "${PROJECT_NAMESPACE}::core"
  "${PROJECT_NAMESPACE}::device"
  "${PROJECT_NAMESPACE}::mechanism"
  "${PROJECT_NAMESPACE}::gui")

target_set_warnings(machine
  ENABLE ALL
  DISABLE Annoying)

set_target_properties(machine PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES 
    CXX_EXTENSIONS NO)

target_enable_lto(machine optimized)
