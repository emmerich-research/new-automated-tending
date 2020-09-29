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
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
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
#include <libalgo/algo.hpp>
#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libmechanism/mechanism.hpp>
#include <libutil/util.hpp>

// 4. Local
#include "util.hpp"

#include "manager.hpp"

#include "window.hpp"

#include "fault-window.hpp"
#include "liquid-control-window.hpp"
#include "liquid-status-window.hpp"
#include "logger-window.hpp"
#include "logger-window.inline.hpp"
#include "manual-movement-window.hpp"
#include "metadata-window.hpp"
#include "movement-window.hpp"
#include "plc-trigger-window.hpp"
#include "speed-profile-window.hpp"
#include "status-window.hpp"
#include "system-info-window.hpp"

#endif  // APP_PRECOMPILED_HPP_
