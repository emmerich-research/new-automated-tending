# --------------------------------------------------------------------------------
#                            Misc (no change needed).
# --------------------------------------------------------------------------------
# Have CMake parse the config file, generating the config header, with
# correct definitions. Here only used to make version number available to
# the source code. Include "general_config.h" (no .in suffix) in the source.

set(PROJECT_CONFIG_FILE "conf/config.yaml")
set(PROJECT_STATE_FILE "conf/state.yaml")
set(PROJECT_MOVEMENT_EDGE_FILE "conf/edge.path")
set(PROJECT_MOVEMENT_ZIGZAG_FILE "conf/zigzag.path")

# configure_file (
#   "${PROJECT_SOURCE_DIR}/include/common.h.in"
#   "${PROJECT_BINARY_DIR}/common.h")

# Copy conf folder
# file(COPY "${PROJECT_SOURCE_DIR}/conf"
#      DESTINATION "${PROJECT_BINARY_DIR}")

# configure_file("${PROJECT_SOURCE_DIR}/conf/config.yaml"
#                "${PROJECT_BINARY_DIR}/conf/config.yaml"
#                COPYONLY)

# configure_file("${PROJECT_SOURCE_DIR}/conf/state.yaml"
#                "${PROJECT_BINARY_DIR}/conf/state.yaml"
#                COPYONLY)

# configure_file("${PROJECT_SOURCE_DIR}/conf/edge.path"
#                "${PROJECT_BINARY_DIR}/conf/edge.path"
#                COPYONLY)

# configure_file("${PROJECT_SOURCE_DIR}/conf/zigzag.path"
#                "${PROJECT_BINARY_DIR}/conf/zigzag.path"
#                COPYONLY)

# add the binary tree to the search path for include files
# so that we will find config.h
include_directories("${PROJECT_BINARY_DIR}")

# Ask CMake to output a compile_commands.json file for use with things like Vim YCM.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
