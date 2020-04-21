add_subdirectory(${PROJECT_SOURCE_DIR}/external/filesystem EXCLUDE_FROM_ALL)
add_library(fs::fs ALIAS ghc_filesystem)
