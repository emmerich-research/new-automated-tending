#ifndef LIB_CORE_LIBCORE_HPP_
#define LIB_CORE_LIBCORE_HPP_

/** @file core.hpp
 *  @brief Precompiled header for faster project compilation
 *
 * Every source file must include this file whether precompiled feature is
 * enabled or not
 */

#pragma GCC system_header

// 1. STL
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

// 2. Vendors
// 2.1. Spdlog
#include <spdlog/async.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// 2.2. TOML
#include <toml.hpp>

// 3. Inside Project
#include <libutil/util.hpp>

// 4. Local
#include "common.hpp"

#include "init.hpp"

#include "allocation.hpp"
#include "allocation.inline.hpp"

#include "config.hpp"
#include "logger.hpp"
#include "state.hpp"

#endif
