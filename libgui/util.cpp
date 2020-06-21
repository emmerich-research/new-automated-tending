#include "gui.hpp"

#include "util.hpp"

#include <tuple>

NAMESPACE_BEGIN

namespace gui {
std::tuple<unsigned int, unsigned int> get_window_size() {
  int width, height;

  glfwGetWindowSize(window(), &width, &height);

  return std::tuple(static_cast<unsigned int>(width),
                    static_cast<unsigned int>(height));
}

void reset_window_pos() {
  ImGui::SetNextWindowPos(ImVec2{0, 0});
}
}  // namespace gui

NAMESPACE_END
