# set(BUILD_SHARED_LIBS OFF  CACHE BOOL "" FORCE)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard whose features are requested to build all targets." FORCE)
set(toml11_BUILD_TEST OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/toml EXCLUDE_FROM_ALL)
