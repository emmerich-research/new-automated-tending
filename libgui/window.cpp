#include "gui.hpp"

#include "window.hpp"

NAMESPACE_BEGIN

namespace gui {
Window::Window(const char*             name,
               float                   width,
               float                   height,
               const ImGuiWindowFlags& flags)
    : name_{name}, width_{width}, height_{height}, flags_{flags} {
  ImGui::SetNextWindowSize(ImVec2{width, height});
}

Window::~Window() {}

void Window::render(const Manager* manager) {
  if (!ImGui::Begin(name(), NULL, flags())) {
    ImGui::End();
    return;
  }

  show(manager);

  ImGui::End();
}

void Window::after_render([[maybe_unused]] const Manager* manager) {}
}  // namespace gui

NAMESPACE_END
