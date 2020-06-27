#include "core.hpp"

#include "logger.hpp"

#include <utility>

#include <spdlog/async.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

NAMESPACE_BEGIN

namespace impl {
LoggerImpl::LoggerImpl(const impl::ConfigImpl* config) : name_{config->name()} {
  DEBUG_ONLY(obj_name_ = "LoggerImpl");

  spdlog::init_thread_pool(8192, 1);

  auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  stdout_sink->set_level(config->debug() ? spdlog::level::debug
                                         : spdlog::level::info);

  fs::create_directory(LOGS_DIR);

  std::string path = fmt::format("{}/{}.log", LOGS_DIR, APP_NAME);
  auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      path, 1024 * 1024 * 15, 3);
  rotating_sink->set_level(spdlog::level::trace);

  std::vector<spdlog::sink_ptr> sinks{stdout_sink, rotating_sink};

  logger_ = spdlog::get(name_);
  if (!logger_) {
    logger_ = std::make_shared<spdlog::async_logger>(
        name_, begin(sinks), end(sinks), spdlog::thread_pool(),
        spdlog::async_overflow_policy::block);

    spdlog::register_logger(logger_);
  }

  logger_->set_pattern("[%d/%m/%C %T][%n][%^%l%$] %v");

  logger_->set_level(spdlog::level::trace);
  logger_->flush_on(spdlog::level::trace);
}

LoggerImpl::~LoggerImpl() {
  spdlog::drop_all();
  spdlog::shutdown();
}
}  // namespace impl

NAMESPACE_END
