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

class GLFWwindow;

NAMESPACE_BEGIN

namespace gui {
// forward declarations
class Manager;
class Window;

class Manager : public StackObj {
 public:
  typedef GLFWwindow   MainWindow;
  typedef GLFWerrorfun ErrorCallback;
  typedef GLFWkeyfun   KeyCallback;
  /**
   * Manager constructor
   *
   * @param num_window    number of window (default: 5)
   */
  Manager(ImVec4      clear_color = ImVec4{0.45f, 0.55f, 0.60f, 1.00f},
          std::size_t num_window = 5);
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
    windows().push_back(new T(std::forward<Args>(args)...));
  }
  /**
   * Init GLFW, OpenGL, and other initializations
   *
   * @param name   main window name
   * @param width  main window width
   * @param height main window height
   */
  void init(const char* name, int width, int height);
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
   * Exit status
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
   * Num of windows
   */
  inline const std::size_t& num_window() const { return num_window_; }
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
   * Active status
   */
  bool active_;
  /**
   * Exit status
   */
  bool exited_;
  /**
   * Num of window
   */
  std::size_t num_window_;
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
