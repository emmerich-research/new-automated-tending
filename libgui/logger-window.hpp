#ifndef LIB_GUI_LOGGER_WINDOW_HPP_
#define LIB_GUI_LOGGER_WINDOW_HPP_

#include <array>
#include <mutex>

#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>

#include <libcore/core.hpp>

#include "window.hpp"

NAMESPACE_BEGIN

namespace gui {
// forward declarations
class Manager;

template <typename Mutex>
class LoggerWindow;

using LoggerWindowMT = LoggerWindow<std::mutex>;
using LoggerWindowST = LoggerWindow<spdlog::details::null_mutex>;

template <typename Mutex>
class LoggerWindow : public Window, public spdlog::sinks::base_sink<Mutex> {
 public:
  /**
   * Logger Window constructor
   *
   * @param width  window width
   * @param height window height
   * @param flags  window flags
   */
  LoggerWindow(float                   width = 500,
               float                   height = 100,
               const ImGuiWindowFlags& flags = 0);
  /**
   * Logger Window destructor
   */
  virtual ~LoggerWindow() override;
  /**
   * Show contents
   *
   * @param manager ui manager
   */
  virtual void show(Manager* manager) override;

 private:
  /**
   * Buffer Size
   */
  static constexpr std::size_t BufferSize = 1000;
  /**
   * Buffer type
   */
  typedef struct BufferType {
    std::string               message;
    spdlog::level::level_enum level;
  } BufferType;
  /**
   * Buffer container Type
   */
  typedef std::array<BufferType, BufferSize> Buffer;
  /**
   * Get string buffer
   *
   * @return string buffer
   */
  inline Buffer& buffer() { return buffer_; }
  /**
   * Get string buffer (const)
   *
   * @return string buffer (const)
   */
  inline const Buffer& buffer() const { return buffer_; }
  /**
   * Get string on specified index
   */
  inline const BufferType& buffer(std::size_t idx) const {
    massert(idx < count(), "sanity");
    return buffer_[idx];
  }
  /**
   * Get buffer count
   *
   * @return buffer count
   */
  inline const std::size_t& count() const { return count_; }
  /**
   * Get log level to show
   *
   * @return log level to show
   */
  inline const spdlog::level::level_enum& level() const { return level_; }

 protected:
  /**
   * Sink for spdlog instance
   */
  void sink_it_(const spdlog::details::log_msg& msg) override;
  /**
   * Flush log from spdlog instance
   */
  void flush_() override;

 private:
  /**
   * Text Buffer
   */
  Buffer buffer_;
  /**
   * Num of item in buffer
   */
  std::size_t count_;
  /**
   * Current level to show
   */
  spdlog::level::level_enum level_;
};
}  // namespace gui

NAMESPACE_END

#endif  // LIB_GUI_LOGGER_WINDOW_HPP_
