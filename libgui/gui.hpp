#ifndef LIB_GUI_GUI_HPP_
#define LIB_GUI_GUI_HPP_

/** @file gui.hpp
 *  @brief Precompiled header for faster project compilation
 *
 * Every source file must include this file whether precompiled feature is
 * enabled or not
 */

#if defined(X_H)
#undef None
#endif

// 1. STL
#include <exception>
#include <iostream>
#include <tuple>

// 2. Vendor

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

#include <GL/gl.h>
#endif

// 2.3. GLFW
#include <GLFW/glfw3.h>

// 3. Inside projects
#include <libutil/util.hpp>

#include <libcore/core.hpp>

#include <libalgo/algo.hpp>

// 4. Local
#include "init.hpp"

#include "util.hpp"
#include "widget.hpp"
#include "window.hpp"

#endif  // LIB_GUI_GUI_HPP_
