#ifndef APP_PRECOMPILED_HPP_
#define APP_PRECOMPILED_HPP_

#pragma GCC system_header

// 1. STD
#include <cstdlib>
#include <iostream>

// 2. vendor

// 2.1. AFSM
#include <afsm/fsm.hpp>

// 3. Internal
#include <libcore/core.hpp>
#include <libdevice/device.hpp>
#include <libmechanism/mechanism.hpp>
#include <libutil/util.hpp>

// 4. Local
#include "state.hpp"
#include "state.inline.hpp"

#include "event.hpp"

#include "action.hpp"
#include "action.inline.hpp"

#include "guard.hpp"
#include "guard.inline.hpp"

#endif  // APP_PRECOMPILED_HPP_
