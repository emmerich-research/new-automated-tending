#ifndef LIB_UTIL_FILESYSTEM_HPP_
#define LIB_UTIL_FILESYSTEM_HPP_

/**
 * @file filesystem.hpp
 * @brief Filesystem class definitions
 */

// #if defined(__cplusplus) && __cplusplus >= 201703L && \
//     defined(__has_include) && !defined(__APPLE__)
// #if __has_include(<filesystem>)
// #define GHC_USE_STD_FS
// #include <filesystem>
// namespace fs {
// using namespace std::filesystem;
// using ifstream = std::ifstream;
// using ofstream = std::ofstream;
// using fstream = std::fstream;
// }  // namespace fs
// #endif
// #endif

// #ifndef GHC_USE_STD_FS
#include <ghc/filesystem.hpp>

namespace fs {
using namespace ghc::filesystem;
using ifstream = ghc::filesystem::ifstream;
using ofstream = ghc::filesystem::ofstream;
using fstream = ghc::filesystem::fstream;
}  // namespace fs

// #endif

#endif  // LIB_CORE_FILESYSTEM_HPP_
