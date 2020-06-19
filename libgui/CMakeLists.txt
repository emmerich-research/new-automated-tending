project(gui)

find_package(SDL2 REQUIRED)

set(IMGUI_VERSION 1.77)

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/external/imgui")

find_package(ImGui ${IMGUI_VERSION} REQUIRED)

ucm_add_files(
  "${IMGUI_INCLUDE_DIR}/imgui.cpp"
  "${IMGUI_INCLUDE_DIR}/imgui_draw.cpp"
  "${IMGUI_INCLUDE_DIR}/imgui_demo.cpp"
  "${IMGUI_INCLUDE_DIR}/imgui_widgets.cpp"
  "${IMGUI_INCLUDE_DIR}/examples/imgui_impl_sdl.cpp"
  "${IMGUI_INCLUDE_DIR}/examples/imgui_impl_opengl3.cpp"

  TO SOURCES)

ucm_add_target(
  NAME gui
  TYPE STATIC
  SOURCES ${SOURCES}
  UNITY CPP_PER_UNITY 20
  PCH_FILE "gui.hpp")

add_library("${PROJECT_NAMESPACE}::gui" ALIAS gui)

target_include_directories(gui PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>"
  "$<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>"
  "$<BUILD_INTERFACE:${IMGUI_INCLUDE_DIR}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_compile_definitions(gui PUBLIC
    GLFW_INCLUDE_NONE)

target_link_libraries(gui PUBLIC
  glad::glad
  ${CMAKE_THREAD_LIBS_INIT}
  "${SDL2_LIBRARIES}"
  "${PROJECT_NAMESPACE}::util"
  "${PROJECT_NAMESPACE}::core"
  "${PROJECT_NAMESPACE}::algo")

add_definitions(-DGLFW_INCLUDE_NONE)

target_set_warnings(gui
  ENABLE ALL
  # AS_ERROR ALL
  DISABLE Annoying)

set_target_properties(gui PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

target_enable_lto(gui optimized)
