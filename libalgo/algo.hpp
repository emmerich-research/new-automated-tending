#ifndef LIB_ALGO_ALGO_HPP_
#define LIB_ALGO_ALGO_HPP_

/** @file algo.hpp
 *  @brief Precompiled header for faster project compilation
 *
 * Every source file must include this file whether precompiled feature is
 * enabled or not
 */

// 1. STL
#include <cstddef>
#include <functional>
#include <future>
#include <map>
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

// 2. Vendor

// 3. Inside projects
#include <libcore/core.hpp>

// 4. Local

// 4.1. Instance Registry
#include "instance_registry.hpp"
#include "instance_registry.inline.hpp"

// 4.2. Thread Pool
#include "thread_pool.hpp"
#include "thread_pool.inline.hpp"

#endif  // LIB_ALGO_ALGO_HPP_
