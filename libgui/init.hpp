#ifndef LIB_GUI_INIT_HPP_
#define LIB_GUI_INIT_HPP_

/** @file init.hpp
 *  @brief GUI initialization
 *
 *  GUI initialization
 */

#include <libcore/core.hpp>

// forward declaration
struct GLFWwindow;

NAMESPACE_BEGIN

extern GLFWwindow* window_;

static auto window = []() { return window_; };

/**
 * Initialize GUI
 *
 * Setup GLFW and IMGUI
 *
 * @return  ATM_STATUS ATM_OK or ATM_ERR
 */
ATM_STATUS initialize_gui();

/**
 * Destroy GUI
 */
void destroy_gui();

NAMESPACE_END

#endif  // LIB_GUI_INIT_HPP_
