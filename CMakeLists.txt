cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)


# Definitions
set(PROJECT_TITLE           "automated-tending-machine")
set(PROJECT_ALT_TITLE       "Emmerich AutomatedTendingMachine")
set(PROJECT_DESCRIPTION     "Emmerich Automated Tending Machine")
set(PROJECT_AUTHOR          "emmerich")
set(PROJECT_NAMESPACE       ${PROJECT_AUTHOR})
set(PROJECT_VERSION_MAJOR   0)
set(PROJECT_VERSION_MINOR   0)
set(PROJECT_VERSION_PATCH   1)
set(PROJECT_VERSION         "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

project(${PROJECT_TITLE}
  VERSION ${PROJECT_VERSION}
  DESCRIPTION ${PROJECT_DESCRIPTION}
  LANGUAGES CXX)

option(ENABLE_WARNINGS_SETTINGS "Allow target_set_warnings to add flags and defines.
                                 Set this to OFF if you want to provide your own warning parameters." ON)
option(ENABLE_LTO "Enable link time optimization" ON)
option(ENABLE_DOCTESTS "Include tests in the library. Setting this to OFF will remove all doctest related code. Tests in tests/*.cpp will still be enabled." OFF)
option(BUILD_SHARED_LIBS "Build shared libs in order for some libraries to work" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include stuffs
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(GNUInstallDirs)
include(AFSM)
include(ConfigSafeGuards)
include(Colors)
include(Cotire)
include(CTest)
include(Doctest)
include(Documentation)
include(FetchContent)
include(Filesystem)
include(Glad)
include(LTO)
include(Misc)
include(Spdlog)
include(Toml)
include(Ucm)
include(Warnings)

# enable unity build
set(UCM_UNITY_BUILD ON)

# Check for LTO support.
find_lto(CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(Boost_USE_DEBUG_LIBS         OFF) # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)

enable_testing()

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
  add_definitions(-DNDEBUG)
endif()

add_definitions(-DATM_COMPILATION)

# Add PIGPIO lib or not
set(RASPI_LIB "")

EXECUTE_PROCESS(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE arch)

if (${arch} MATCHES "^arm")
  find_package(pigpio REQUIRED)
  LIST(APPEND RASPI_LIB "pigpio")
  LIST(APPEND RASPI_LIB "rt")

  # check version
  # glxinfo | grep "OpenGL version string:" | grep -E -o "[0-9]\.[0-9]" | head -1

  # to make OpenGL3 + ImGUI works
  # OpenGL in Raspi is basically OpenGL2
  # there is error with :
  # GL_SAMPLER_BINDING (needs to be OpenGL3)
  # https://github.com/ocornut/imgui/pull/1806

  set(GLAD_API "gl=2.1,gles2=2.0" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest" FORCE)
  set(OPENGL2 ON)
  set(OPENGL3 OFF)
else()
  add_definitions(-DMOCK_GPIO)
  set(GLAD_API "" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest" FORCE)
  set(OPENGL3 OFF)
  set(OPENGL3 ON)
endif()

find_package(Threads REQUIRED)

add_subdirectory(libutil)
add_subdirectory(libcore)
add_subdirectory(libalgo)
add_subdirectory(libgui)
add_subdirectory(libdevice)
add_subdirectory(libmechanism)
add_subdirectory(driver)
add_subdirectory(app)
