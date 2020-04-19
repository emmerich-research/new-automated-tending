#ifndef LIB_CORE_LOGGER_HPP_
#define LIB_CORE_LOGGER_HPP_

/** \file logger.hpp
 *  \brief Logger singleton class definition
 *
 * Project's logger
 */

#include <memory>
#include <string>

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

/**
 * @brief Singleton of impl::StateImpl class
 *
 * Public and singleton class of impl::StateImpl class
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class Logger : StaticObj {
 public:
  /**
   * Logger singleton initialization
   *
   * @param  args    arguments are same with impl::LoggerImpl()
   * @return impl::LoggerImpl pointer
   */
  template <typename... Args>
  inline static impl::LoggerImpl* create(Args... args);

  /**
   * Get impl::LoggerImpl pointer
   *
   * @return impl::LoggerImpl pointer that has been initialized
   */
  inline static impl::LoggerImpl* get();

 private:
  static impl::LoggerImpl* instance_;
};

namespace impl {
class LoggerImpl : public StackObj {
  template <typename... Args>
  friend LoggerImpl* Logger::create(Args... args);

 public:
  inline const std::shared_ptr<spdlog::logger>& getLogger() const {
    return logger_;
  }

  inline spdlog::level::level_enum level() const { return logger_->level(); }

  inline void set_level(const spdlog::level::level_enum& log_level) {
    logger_->set_level(log_level);
  }

  template <typename... Args>
  inline void trace(fmt::basic_string_view<char> fmt, const Args&... args) {
    logger_->trace(fmt, args...);
  }

  template <typename... Args>
  inline void debug(fmt::basic_string_view<char> fmt, const Args&... args) {
    logger_->debug(fmt, args...);
  }

  template <typename... Args>
  inline void info(fmt::basic_string_view<char> fmt, const Args&... args) {
    logger_->info(fmt, args...);
  }

  template <typename... Args>
  inline void warn(fmt::basic_string_view<char> fmt, const Args&... args) {
    logger_->warn(fmt, args...);
  }

  template <typename... Args>
  inline void error(fmt::basic_string_view<char> fmt, const Args&... args) {
    logger_->error(fmt, args...);
  }

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
  const std::string               name_;
  std::shared_ptr<spdlog::logger> logger_;
};
}  // namespace impl

template <typename... Args>
inline impl::LoggerImpl* Logger::create(Args... args) {
  massert(instance_ == nullptr, "create only can be called once");
  if (instance_ == nullptr) {
    static impl::LoggerImpl logger(args...);
    instance_ = &logger;
  }
  massert(instance_ != nullptr, "sanity check");
  return instance_;
}

inline impl::LoggerImpl* Logger::get() {
  massert(instance_ != nullptr, "can only be called if it is initialized");
  return instance_;
}

NAMESPACE_END

#endif
