#ifndef LIB_GUI_UTIL_HPP_
#define LIB_GUI_UTIL_HPP_

#include <external/imgui/imgui.h>

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace gui {
namespace util {
namespace size {
ImVec2 h_wide(float height = 50.0f);
}

namespace color {
extern ImVec4 red;
extern ImVec4 green;
extern ImVec4 blue;
}  // namespace color

bool button(const char*   label,
            unsigned int  status_id,
            bool          active = false,
            const ImVec2& size = {0, 0});
void status_button(const char*   label,
                   unsigned int  status_id,
                   bool          active,
                   const ImVec2& size = {0, 0});
}  // namespace util
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_UTIL_HPP_
