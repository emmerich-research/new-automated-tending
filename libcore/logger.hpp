#ifndef LIB_CORE_LOGGER_HPP_
#define LIB_CORE_LOGGER_HPP_

/** @file logger.hpp
 *  @brief Logger singleton class definition
 *
 * Project's logger
 */

#include <memory>
#include <string>
#include <utility>

#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include "common.hpp"

#include "allocation.hpp"
#include "macros.hpp"

#include "config.hpp"

NAMESPACE_BEGIN

namespace impl {
class LoggerImpl;
}

/** impl::LoggerImpl singleton class using StaticObj */
using Logger = StaticObj<impl::LoggerImpl>;

namespace impl {
/**
 * @brief Logger implementation.
 *        This is a class wrapper that should not be instantiated and accessed
 * publicly.
 *
 * Machine's console and file logger
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class LoggerImpl : public StackObj {
  template <class LoggerImpl>
  template <typename... Args>
  friend ATM_STATUS StaticObj<LoggerImpl>::create(Args&&... args);

 public:
  /**
   * Get shared_ptr of spdlog logger
   */
  inline const std::shared_ptr<spdlog::logger>& getLogger() const {
    return logger_;
  }
  /**
   * Get current level of spdlog logger
   */
  inline spdlog::level::level_enum level() const { return logger_->level(); }
  /**
   * Set spdlog logger level
   */
  inline void set_level(const spdlog::level::level_enum& log_level) {
    logger_->set_level(log_level);
  }
  /**
   * Output log message in trace level
   *
   * @param fmt   fmt::fmt format syntax, see https://fmt.dev/latest/syntax.html
   * for more details
   * @param args  formatted message
   */
  template <typename... Args>
  inline void trace(fmt::basic_string_view<char> fmt, const Args&... args) {
    logger_->trace(fmt, args...);
  }
  /**
   * Output log message in debug level
   *
   * @param fmt   fmt::fmt format syntax, see https://fmt.dev/latest/syntax.html
   * for more details
   * @param args  formatted message
   */
  template <typename... Args>
  inline void debug(fmt::basic_string_view<char> fmt, const Args&... args) {
    logger_->debug(fmt, args...);
  }
  /**
   * Output log message in info level
   *
   * @param fmt   fmt::fmt format syntax, see https://fmt.dev/latest/syntax.html
   * for more details
   * @param args  formatted message
   */
  template <typename... Args>
  inline void info(fmt::basic_string_view<char> fmt, const Args&... args) {
    logger_->info(fmt, args...);
  }
  /**
   * Output log message in warn level
   *
   * @param fmt   fmt::fmt format syntax, see https://fmt.dev/latest/syntax.html
   * for more details
   * @param args  formatted message
   */
  template <typename... Args>
  inline void warn(fmt::basic_string_view<char> fmt, const Args&... args) {
    logger_->warn(fmt, args...);
  }
  /**
   * Output log message in error level
   *
   * @param fmt   fmt::fmt format syntax, see https://fmt.dev/latest/syntax.html
   * for more details
   * @param args  formatted message
   */
  template <typename... Args>
  inline void error(fmt::basic_string_view<char> fmt, const Args&... args) {
    logger_->error(fmt, args...);
  }
  /**
   * Output log message in critical level
   *
   * @param fmt   fmt::fmt format syntax, see https://fmt.dev/latest/syntax.html
   * for more details
   * @param args  formatted message
   */
  template <typename... Args>
  void critical(fmt::basic_string_view<char> fmt, const Args&... args) {
    logger_->critical(fmt, args...);
  }

 private:
  /**
   * ConfigImpl Constructor
   *
   * Initialize log file and log to screen functionality
   *
   * @param config   impl::ConfigImpl pointer
   */
  explicit LoggerImpl(const impl::ConfigImpl* config);
  /**
   * ConfigImpl Destructor
   *
   * Will destroy all the spdlog logger instances
   */
  ~LoggerImpl();

 private:
  /**
   * Logger instance name
   */
  const std::string name_;
  /**
   * Shared pointer of spdlog logger
   */
  std::shared_ptr<spdlog::logger> logger_;
};
}  // namespace impl

NAMESPACE_END

#endif
