#include "util.hpp"

#include <iostream>

#ifdef PROJECT_DEBUG
void _massert(const char* expr_str,
              bool        expr,
              const char* file,
              int         line,
              const char* msg) {
  if (!expr) {
    std::cerr << "[" << file << ":" << line << "] assertion failed: " << msg
              << std::endl
              << "expected: " << expr_str << "\n";
    std::abort();
  }
}
#endif
