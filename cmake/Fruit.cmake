set(FRUIT_USES_BOOST OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON)

string(TOLOWER "${CMAKE_BUILD_TYPE}" initial_cmake_build_type_tolower)
if (initial_cmake_build_type_tolower STREQUAL "coverage")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/external/fruit EXCLUDE_FROM_ALL)
target_include_directories(fruit INTERFACE ${PROJECT_SOURCE_DIR}/external/fruit/include)
target_include_directories(fruit INTERFACE ${CMAKE_BINARY_DIR}/external/fruit/include)

string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_tolower)
if (cmake_build_type_tolower STREQUAL "debug" AND initial_cmake_build_type_tolower STREQUAL "coverage")
  set(CMAKE_BUILD_TYPE "Coverage" CACHE STRING "" FORCE)
endif()