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

// 2. Vendor
// 2.1. Glad
#include <glad/glad.h>

// 2.2. SDL
#include <SDL.h>

// 2.3. ImGui
#include <external/imgui/imgui.h>

#include <external/imgui/imconfig.h>
// #include <external/imgui/imgui_internal.h>
#include <external/imgui/imstb_rectpack.h>
#include <external/imgui/imstb_truetype.h>

#include <external/imgui/examples/imgui_impl_opengl3.h>
#include <external/imgui/examples/imgui_impl_sdl.h>

// 3. Inside projects
#include <libutil/util.hpp>

#include <libcore/core.hpp>

#include <libalgo/algo.hpp>

// 4. Local

#endif  // LIB_GUI_GUI_HPP_
