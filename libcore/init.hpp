#ifndef LIB_CORE_INIT_HPP_
#define LIB_CORE_INIT_HPP_

/** @file init.hpp
 *  @brief Core initialization
 */

#include "common.hpp"

NAMESPACE_BEGIN

/**
 * Initialize core
 *
 * Will initialize these singletons :
 * - Config
 * - Logger
 * - State
 *
 * @return  ATM_STATUS ATM_OK or ATM_ERR
 */
ATM_STATUS initialize_core();

/**
 * Destroy core
 *
 * @return void
 */
void destroy_core();

NAMESPACE_END

#endif
