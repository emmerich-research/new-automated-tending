project(algo)

# ucm_add_files(
#   "instance_registry.cpp"

#   TO SOURCES)

# ucm_add_target(
#   NAME algo
#   TYPE STATIC
#   SOURCES ${SOURCES})
  # UNITY CPP_PER_UNITY 20
  # PCH_FILE "device.hpp")

add_library(algo INTERFACE)
 
add_library("${PROJECT_NAMESPACE}::algo" ALIAS algo)

target_include_directories(algo INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(algo INTERFACE
  ${CMAKE_THREAD_LIBS_INIT}
  ${RASPI_LIB}
  "${PROJECT_NAMESPACE}::core")

# target_set_warnings(algo
  # ENABLE ALL
  # AS_ERROR ALL
  # DISABLE Annoying)

# set_target_properties(algo PROPERTIES
#     CXX_STANDARD 17
#     CXX_STANDARD_REQUIRED YES
#     CXX_EXTENSIONS NO)

# target_enable_lto(algo optimized)
