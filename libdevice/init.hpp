#ifndef LIB_DEVICE_INIT_HPP_
#define LIB_DEVICE_INIT_HPP_

/** @file init.hpp
 *  @brief Devices initialization
 *
 *  Devices initialization
 */

#include <libcore/core.hpp>

NAMESPACE_BEGIN

/**
 * Initialize devices
 *
 * There are 3 types of device:
 * - Digital
 * - Analog
 * - PWM
 *
 * @return  ATM_STATUS ATM_OK or ATM_ERR
 */
ATM_STATUS initialize_device();

/**
 * Destroy devices
 */
void destroy_device();

NAMESPACE_END

#endif  // LIB_DEVICE_INIT_HPP_
