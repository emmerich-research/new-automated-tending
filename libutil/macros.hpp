#ifndef LIB_UTIL_MACROS_HPP_
#define LIB_UTIL_MACROS_HPP_

/** @file macros.hpp
 *  @brief Macros definitions
 */

/**
 * @def DEBUG_ONLY(code)
 *
 * @param code code that will be executed in DEBUG environment
 */

/**
 * @def NOT_DEBUG(code)
 *
 * @param code code that will be executed in NON-DEBUG environment
 */

/**
 * @def massert(Expr, Msg)
 *
 * @param Expr expression that needs to be tested
 * @param Msg  message for this assert
 */

#ifdef PROJECT_DEBUG
#define DEBUG_ONLY_DEFINITION(code) code
#define DEBUG_ONLY(code) \
  do {                   \
    code;                \
  } while (0)
#define NOT_DEBUG_DEFINITION(code)
#define NOT_DEBUG(code) static_cast<void>(0)
#define massert(Expr, Msg) _massert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#define DEBUG_ONLY_DEFINITION(code)
#define DEBUG_ONLY(code) static_cast<void>(0)
#define NOT_DEBUG_DEFINITION(code) code
#define NOT_DEBUG(code) \
  do {                  \
    code;               \
  } while (0)
#define massert(Expr, Msg) static_cast<void>(0)
#endif

#ifdef PROJECT_DEBUG
/**
 * @brief Assertion with message.
 *        Enabled only in development.
 *
 * taken from https://stackoverflow.com/a/37264642/6808347 <br />
 * credits to Eugene Magdalits <br />
 * with some changes by Ray Andrew <raydreww@gmail.com>
 *
 * This function will output message and abort if expr equal false
 *
 * @param expr_str expression in string
 * @param expr     result of expression
 * @param file     filename that execute this function
 * @param line     number of line in the file that execute this function
 * @param msg      assertion message
 */
void _massert(const char* expr_str,
              bool        expr,
              const char* file,
              int         line,
              const char* msg);
#endif

#endif  // LIB_UTIL_MACROS_HPP_
