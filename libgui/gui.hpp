#ifndef APP_PRECOMPILED_HPP_
#define APP_PRECOMPILED_HPP_

/** @file precompiled.hpp
 *  @brief Precompiled header for faster project compilation
 *
 * Every source file must include this file whether precompiled feature is
 * enabled or not
 */

#pragma GCC system_header

// 1. STD
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

// 2. Vendors
// 2.1. ImGui
#include <external/imgui/imgui.h>

#include <external/imgui/imconfig.h>
// #include <external/imgui/imgui_internal.h>
#include <external/imgui/imstb_rectpack.h>
#include <external/imgui/imstb_truetype.h>

#include <external/imgui/examples/imgui_impl_glfw.h>

#if defined(OPENGL3_EXIST)
#include <external/imgui/examples/imgui_impl_opengl3.h>

// 2.2. Glad
#include <glad/glad.h>
#elif defined(OPENGL2_EXIST)
#include <external/imgui/examples/imgui_impl_opengl2.h>

// 2.3. GL
#include <GL/gl.h>
#endif

// 2.4. GLFW
#include <GLFW/glfw3.h>

// 3. Internal projects
#include <libcore/core.hpp>
#include <libutil/util.hpp>

// 4. Local
#include "manager.hpp"

#include "window.hpp"

#endif  // APP_PRECOMPILED_HPP_
