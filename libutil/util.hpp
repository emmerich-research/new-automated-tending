#ifndef LIBUTIL_UTIL_HPP_
#define LIBUTIL_UTIL_HPP_

/** @file core.hpp
 *  @brief Precompiled header for faster project compilation
 *
 * Every source file must include this file whether precompiled feature is
 * enabled or not
 */

// 1. STL
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

// 2. Vendors
// 2.1. GHC Filesystem
#include <ghc/filesystem.hpp>

// 3. Local
#include "boolean.hpp"
#include "filesystem.hpp"
#include "macros.hpp"
#include "pair.hpp"
#include "timer.hpp"

#endif  // LIBUTIL_UTIL_HPP_
