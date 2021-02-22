#ifndef LIB_GUI_LOGGER_WINDOW_INLINE_HPP_
#define LIB_GUI_LOGGER_WINDOW_INLINE_HPP_

#include "logger-window.hpp"

#include <cstring>

#include <external/imgui/misc/cpp/imgui_stdlib.h>

#include "util.hpp"

NAMESPACE_BEGIN

namespace gui {
template <typename Mutex>
LoggerWindow<Mutex>::LoggerWindow(float                   width,
                                  float                   height,
                                  const ImGuiWindowFlags& flags)
    : Window{"Logger", width, height,
             ImGuiWindowFlags_AlwaysVerticalScrollbar | flags},
      count_{0},
      level_{spdlog::level::trace} {}

template <typename Mutex>
LoggerWindow<Mutex>::~LoggerWindow() {
  // buffer().clear();
}

template <typename Mutex>
void LoggerWindow<Mutex>::show(Manager* manager) {
  // const ImGuiInputTextFlags flags = ImGuiInputTextFlags_ReadOnly;
  const ImVec2 window_size = ImGui::GetWindowSize();
  float        wrap_width = window_size.x - 24;
  // const float  footer_height_to_reserve =
  //     ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
  ImGui::PushFont(manager->logging_font());
  {
    ImGui::RadioButton("all", reinterpret_cast<int*>(&level_),
                       spdlog::level::trace);
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Text, util::color::red);
    ImGui::PushStyleColor(ImGuiCol_CheckMark, util::color::red);
    ImGui::RadioButton("error", reinterpret_cast<int*>(&level_),
                       spdlog::level::err);
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Text, util::color::blue);
    ImGui::PushStyleColor(ImGuiCol_CheckMark, util::color::blue);
    ImGui::RadioButton("info", reinterpret_cast<int*>(&level_),
                       spdlog::level::info);
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Text, util::color::green);
    ImGui::PushStyleColor(ImGuiCol_CheckMark, util::color::green);
    ImGui::RadioButton("debug", reinterpret_cast<int*>(&level_),
                       spdlog::level::debug);
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
  }

  {
    ImGui::BeginChild("ScrollingRegion", ImVec2{0, -FLT_MIN}, false,
                      ImGuiWindowFlags_NoScrollbar);
    ImGui::SetScrollX(0);

    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                        ImVec2(4, 1));  // Tighten spacing
    for (std::size_t i = 0; i < count(); ++i) {
      const auto& payload = buffer(i);
      ImVec4      color;
      bool        has_color = false;

      if ((level() != spdlog::level::trace) && (payload.level != level()))
        continue;

      if (payload.level == spdlog::level::err) {
        // red
        color = util::color::red;
        has_color = true;
      }

      if (payload.level == spdlog::level::info) {
        // blue
        color = util::color::blue;
        has_color = true;
      }

      if (payload.level == spdlog::level::debug) {
        // green
        color = util::color::green;
        has_color = true;
      }

      // render level
      if (has_color)
        ImGui::PushStyleColor(ImGuiCol_Text, color);
      ImGui::Text("[%s]", spdlog::level::to_string_view(payload.level));
      if (has_color)
        ImGui::PopStyleColor();

      ImGui::SameLine();

      // render message
      ImGui::Text(payload.message.c_str(), wrap_width);
    }
    ImGui::PopStyleVar();

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
      ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
  }
  ImGui::PopFont();
}

template <typename Mutex>
void LoggerWindow<Mutex>::sink_it_(const spdlog::details::log_msg& msg) {
  if (count() + 1 >= LoggerWindow::BufferSize) {
    count_ = 0;
  }

  buffer_[count_++] = {std::move(fmt::to_string(msg.payload)), msg.level};
}

template <typename Mutex>
void LoggerWindow<Mutex>::flush_() {}
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_LOGGER_WINDOW_INLINE_HPP_
