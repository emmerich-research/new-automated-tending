#ifndef LIB_GUI_MANAGER_HPP_
#define LIB_GUI_MANAGER_HPP_

/** @file image_texture.hpp
 *  @brief Imgui OpenCV Image texture
 *
 * Imgui OpenCV Image texture
 */

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <string>
#include <type_traits>
#include <vector>

#include <external/imgui/imgui.h>

#include <libcore/core.hpp>
#include <libutil/util.hpp>

#include "window.hpp"

class GLFWwindow;

NAMESPACE_BEGIN

namespace gui {
// forward declarations
class Manager;
// class Window;

class Manager : public StackObj {
 public:
  typedef GLFWwindow   MainWindow;
  typedef GLFWerrorfun ErrorCallback;
  typedef GLFWkeyfun   KeyCallback;
  /**
   * Manager constructor
   *
   * @param name          main window name
   * @param clear_color   background color of main window
   */
  Manager(const std::string& name = "",
          ImVec4             clear_color = ImVec4{0.0f, 0.0f, 0.0f, 1.00f});
  /**
   * Manager destructor
   */
  virtual ~Manager();
  /**
   * Add window to manager
   *
   * @param window  window to be added
   */
  void add_window(Window* window);
  /**
   * Add window to manager
   *
   * @param args arguments to pass
   */
  template <typename T,
            typename... Args,
            typename = std::enable_if_t<std::is_base_of_v<Window, T>>>
  inline void add_window(Args&&... args) {
    massert(active(), "sanity");
    T* window_ptr = new T(std::forward<Args>(args)...);
    windows().push_back(window_ptr);
    // window_ptr->render(this);
  }
  /**
   * Init GLFW, OpenGL, and other initializations
   *
   */
  void init();
  /**
   * Set window name
   *
   * @param name window name to set
   */
  void name(const char* name);
  /**
   * Set window name
   *
   * @param name window name to set
   */
  void name(const std::string& name);
  /**
   * Set clear color
   *
   * @param color color to set
   */
  void clear_color(const ImVec4& color);
  /**
   * Show contents
   */
  void render();
  /**
   * Exit manager
   */
  void exit();
  /**
   * Active status
   *
   * @return active status
   */
  inline bool active() const { return active_; }
  /**
   * GLFW Window exit status
   *
   * @return exit status
   */
  inline bool terminated() const { return terminated_; }
  /**
   * Exit status helper for deallocation
   *
   * @return exit status
   */
  inline bool exited() const { return exited_; }
  /**
   * Check status of events handling
   *
   * @return events handling is active
   */
  bool handle_events();
  /**
   * Set error callback
   *
   * @param error_cb error callback
   */
  void error_callback(const ErrorCallback&& error_cb);
  /**
   * Set key callback
   *
   * @param error_cb error callback
   */
  void key_callback(const KeyCallback&& error_cb);

 protected:
  /**
   * Get main window name
   *
   * @return main window name
   */
  inline const std::string& name() const { return name_; }
  /**
   * Get windows container
   *
   * @return windows container
   */
  inline std::vector<Window*>& windows() { return windows_; }
  /**
   * Get windows container (const)
   *
   * @return windows container (const)
   */
  inline const std::vector<Window*>& windows() const { return windows_; }
  /**
   * Get main window pointer
   *
   * @return pointer to main window
   */
  inline GLFWwindow* window() { return window_; }
  /**
   * Get main window pointer (const)
   *
   * @return pointer to main window (const)
   */
  inline const GLFWwindow* window() const { return window_; }
  /**
   * Get clear color
   *
   * @return clear color
   */
  inline const ImVec4& clear_color() const { return clear_color_; }

 private:
  /**
   * Main window name
   */
  std::string name_;
  /**
   * Active status
   */
  bool active_;
  /**
   * GLFW Window termination status
   */
  bool terminated_;
  /**
   * Exit status
   */
  bool exited_;
  /**
   * Clear color
   */
  ImVec4 clear_color_;
  /**
   * Windows container
   */
  std::vector<Window*> windows_;
  /**
   * Main window
   */
  GLFWwindow* window_ = nullptr;
};
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_MANAGER_HPP_
