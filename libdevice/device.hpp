#ifndef LIB_DEVICE_DEVICE_HPP_
#define LIB_DEVICE_DEVICE_HPP_

/** @file device.hpp
 *  @brief Precompiled header for faster project compilation
 *
 * Every source file must include this file whether precompiled feature is
 * enabled or not
 */

// 1. STL
#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>

// 2. Vendor

// 2.1 Pigpio
#ifndef MOCK_GPIO
#include <pigpio.h>
#endif  // MOCK_GPIO

// 3. Inside projects
#include <libcore/core.hpp>

// 4. Local
#include "gpio.hpp"

// 4.1. Analog Device
#include "analog/analog.hpp"

#endif  // LIB_DEVICE_DEVICE_HPP_
