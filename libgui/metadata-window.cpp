#include "gui.hpp"

#include "metadata-window.hpp"

#include <chrono>
#include <ctime>

#include <libdevice/device.hpp>
#include <libmechanism/mechanism.hpp>

#include "util.hpp"

NAMESPACE_BEGIN

namespace gui {
MetadataWindow::MetadataWindow(float                   width,
                               float                   height,
                               const ImGuiWindowFlags& flags)
    : Window{"Metadata", width, height,
             flags | ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoBackground} {}

MetadataWindow::~MetadataWindow() {}

void MetadataWindow::show([[maybe_unused]] Manager* manager) {
  auto        now = Clock::now();
  std::time_t now_c = Clock::to_time_t(now);
  struct tm*  time = std::localtime(&now_c);
  ImGui::Text("%s, %d", APP_NAME_FULL, 1900 + time->tm_year);
}
}  // namespace gui

NAMESPACE_END
