#ifndef LIB_MECHANISM_MECHANISM_HPP_
#define LIB_MECHANISM_MECHANISM_HPP_

/** @file device.hpp
 *  @brief Precompiled header for faster project compilation
 *
 * Every source file must include this file whether precompiled feature is
 * enabled or not
 */

// 1. STL
#include <memory>
#include <string>
#include <thread>

// 2. Vendor

// 3. Inside projects
#include <libcore/core.hpp>

#include <libalgo/algo.hpp>

#include <libdevice/device.hpp>
// 4. Local

// 4.1. Movement Mechanism
#include "movement.hpp"
#include "movement.inline.hpp"

#endif  // LIB_MECHANISM_MECHANISM_HPP_
