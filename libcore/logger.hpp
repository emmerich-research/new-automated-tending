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

#include <libutil/util.hpp>

#include "common.hpp"

#include "allocation.hpp"

#include "config.hpp"

/**
 * @def LOGGER
 *
 * Get Logger singleton instance
 */
#define LOGGER ns(Logger::get())

/**
 * @def LOG_DEBUG
 *
 * @see impl::LoggerImpl::trace
 *
 * @param args any fmt::fmt format
 */
#define LOG_TRACE(...)              \
  do {                              \
    if (LOGGER != nullptr) {        \
      (LOGGER)->trace(__VA_ARGS__); \
    }                               \
  } while (0)

/**
 * @def LOG_DEBUG
 *
 * @see impl::LoggerImpl::debug
 *
 * @param args any fmt::fmt format
 */
#define LOG_DEBUG(...)              \
  do {                              \
    if (LOGGER != nullptr) {        \
      (LOGGER)->debug(__VA_ARGS__); \
    }                               \
  } while (0)

/**
 * @def LOG_INFO
 *
 * @see impl::LoggerImpl::info
 *
 * @param args any fmt::fmt format
 */
#define LOG_INFO(...)              \
  do {                             \
    if (LOGGER != nullptr) {       \
      (LOGGER)->info(__VA_ARGS__); \
    }                              \
  } while (0)

/**
 * @def LOG_WARN
 *
 * @see impl::LoggerImpl::warn
 *
 * @param args any fmt::fmt format
 */
#define LOG_WARN(...)              \
  do {                             \
    if (LOGGER != nullptr) {       \
      (LOGGER)->warn(__VA_ARGS__); \
    }                              \
  } while (0)

/**
 * @def LOG_ERROR
 *
 * @see impl::LoggerImpl::error
 *
 * @param args any fmt::fmt format
 */
#define LOG_ERROR(...)              \
  do {                              \
    if (LOGGER != nullptr) {        \
      (LOGGER)->error(__VA_ARGS__); \
    }                               \
  } while (0)

/**
 * @def LOG_CRITICAL
 *
 * @see impl::LoggerImpl::critical
 *
 * @param args any fmt::fmt format
 */
#define LOG_CRITICAL(...)              \
  do {                                 \
    if (LOGGER != nullptr) {           \
      (LOGGER)->critical(__VA_ARGS__); \
    }                                  \
  } while (0)

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
   *
   * @return shared_ptr<spdlog::logger>
   */
  inline const std::shared_ptr<spdlog::logger>& getLogger() const {
    return logger_;
  }
  /**
   * Get current level of spdlog logger
   *
   * @return spdlog logger level
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
  inline void critical(fmt::basic_string_view<char> fmt, const Args&... args) {
    logger_->critical(fmt, args...);
  }

 private:
  /**
   * LoggerImpl Constructor
   *
   * Initialize log file and log to screen functionality
   *
   * @param config   impl::ConfigImpl pointer
   */
  explicit LoggerImpl(const impl::ConfigImpl* config);
  /**
   * LoggerImpl Destructor
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
