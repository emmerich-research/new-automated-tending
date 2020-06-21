#ifndef LIB_GUI_UTIL_HPP_
#define LIB_GUI_UTIL_HPP_

/** @file util.hpp
 *  @brief Util
 *
 *  Util
 */

#include <utility>

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace gui {
std::tuple<unsigned int, unsigned int> get_window_size();
void                                   reset_window_pos();
}  // namespace gui

NAMESPACE_END

#endif // LIB_GUI_UTIL_HPP_
