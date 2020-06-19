set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glad EXCLUDE_FROM_ALL)
add_library(glad::glad ALIAS glad)
