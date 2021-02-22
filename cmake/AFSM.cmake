# set(BUILD_SHARED_LIBS OFF  CACHE BOOL "" FORCE)
set(AFSM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/afsm)
add_library(afsm::afsm ALIAS afsm)
