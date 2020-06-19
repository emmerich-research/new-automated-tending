add_subdirectory(${PROJECT_SOURCE_DIR}/external/glad EXCLUDE_FROM_ALL)
add_library(glad::glad ALIAS glad)
