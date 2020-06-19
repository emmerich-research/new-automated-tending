#ifndef LIB_MECHANISM_INIT_HPP_
#define LIB_MECHANISM_INIT_HPP_

/** @file init.hpp
 *  @brief Mechanisms initialization
 *
 *  Mechanisms initialization
 */
#include <libcore/core.hpp>

NAMESPACE_BEGIN
/**
 * Initialize mechanisms
 *
 * There is type of mechanism:
 * - Movement
 *
 * @return  ATM_STATUS ATM_OK or ATM_ERR
 */
ATM_STATUS initialize_mechanism();

/**
 * Destroy mechanisms
 */
void destroy_mechanism();

NAMESPACE_END

#endif  // LIB_MECHANISM_INIT_HPP_
