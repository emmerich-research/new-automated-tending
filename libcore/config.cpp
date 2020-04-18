#include "core.hpp"

#include "config.hpp"

#include <utility>

NAMESPACE_BEGIN

impl::ConfigImpl* Config::instance_ = nullptr;

namespace impl {
ConfigImpl::ConfigImpl(const std::string config_path)
    : config_path_(std::move(config_path)) {
  DEBUG_ONLY(obj_name_ = "ConfigImpl");
  config_ = std::make_shared<YAML::Node>(YAML::LoadFile(config_path_));
}
}  // namespace impl

NAMESPACE_END
