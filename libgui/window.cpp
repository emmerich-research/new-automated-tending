#include "gui.hpp"

#include "window.hpp"

#include "manager.hpp"

NAMESPACE_BEGIN

namespace gui {
Window::Window(const char*             name,
               float                   width,
               float                   height,
               const ImGuiWindowFlags& flags)
    : name_{name},
      width_{width},
      height_{height},
      flags_{ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
             ImGuiWindowFlags_NoCollapse |
             ImGuiWindowFlags_NoBringToFrontOnFocus |
             ImGuiWindowFlags_NoFocusOnAppearing |
             ImGuiWindowFlags_NoBringToFrontOnFocus |
             /* ImGuiWindowFlags_NoFocusOnAppearing */ flags} {}

Window::~Window() {}

void Window::render([[maybe_unused]] Manager* manager) {
  // ImGui::SetNextWindowSize(ImVec2{width(), height()});
  if (!ImGui::Begin(name(), NULL, flags())) {
    ImGui::End();
    return;
  }

  show(manager);

  ImGui::End();
}

void Window::after_render([[maybe_unused]] Manager* manager) {}
}  // namespace gui

NAMESPACE_END
