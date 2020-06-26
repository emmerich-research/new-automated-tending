#ifndef LIB_DEVICE_DEVICE_HPP_
#define LIB_DEVICE_DEVICE_HPP_

/** @file device.hpp
 *  @brief Precompiled header for faster project compilation
 *
 * Every source file must include this file whether precompiled feature is
 * enabled or not
 */

// 1. STL
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>

// 2. Vendor

// 2.1 Pigpio
#ifndef MOCK_GPIO
#include <pigpio.h>
#endif  // MOCK_GPIO

// 3. Inside projects
#include <libutil/util.hpp>

#include <libcore/core.hpp>

#include <libalgo/algo.hpp>

// 4. Local
#include "gpio.hpp"

#include "identifier.hpp"
#include "init.hpp"

// 4.1. Analog Device
#include "analog.hpp"

#include "PCF8591.hpp"

// 4.2. Digital Device
#include "digital.hpp"
#include "digital.inline.hpp"
#include "pwm.hpp"

// 4.3. Stepper Device
#include "stepper.hpp"
#include "stepper.inline.hpp"

#include "A4988.hpp"
#include "A4988.inline.hpp"

// 4.4. Shift Register Device
#include "shift_register.hpp"

#endif  // LIB_DEVICE_DEVICE_HPP_
