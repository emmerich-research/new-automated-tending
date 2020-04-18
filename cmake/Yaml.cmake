set(BUILD_SHARED_LIBS OFF  CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/yaml EXCLUDE_FROM_ALL)
add_library(yaml::yaml ALIAS yaml-cpp)
