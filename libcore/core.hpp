#ifndef LIB_CORE_LIBCORE_HPP_
#define LIB_CORE_LIBCORE_HPP_

/** @file core.hpp
 *  @brief Precompiled header for faster project compilation
 *
 * Every source file must include this file whether precompiled feature is
 * enabled or not
 */

// 1. STL
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <thread>

// 2. Vendors
// 2.1. Spdlog
#include <spdlog/async.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// 2.2. YAML
#include <yaml-cpp/yaml.h>

// 3. Local
#include "common.hpp"
#include "macros.hpp"

#include "init.hpp"

#include "allocation.hpp"
#include "allocation.inline.hpp"

#include "config.hpp"
#include "logger.hpp"
#include "state.hpp"
#include "timer.hpp"

#endif
