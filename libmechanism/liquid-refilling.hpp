#ifndef LIB_MECHANISM_LIQUID_REFILLING_HPP_
#define LIB_MECHANISM_LIQUID_REFILLING_HPP_

/** @file liquid-refilling.hpp
 *  @brief Liquid refilling mechanism class definition
 *
 * Liquid refilling mechanism
 */

#include <memory>
#include <string>

#include <libcore/core.hpp>
#include <libdevice/device.hpp>

NAMESPACE_BEGIN

namespace mechanism {
// forward declaration
namespace impl {
class LiquidRefillingImpl;
}

namespace liquid {
enum class status { full, normal, empty, unknown };
}

using LiquidRefilling = StaticObj<impl::LiquidRefillingImpl>;

namespace impl {
class LiquidRefillingImpl : public StackObj {
  template <class LiquidRefillingImpl>
  template <typename... Args>
  friend ATM_STATUS StaticObj<LiquidRefillingImpl>::create(Args&&... args);

 public:
  /**
   * Get active status
   *
   * @return active status
   */
  inline const bool& active() const { return active_; }
  /**
   * Get water level status
   *
   * @return water level status
   */
  liquid::status water_level() const;
  /**
   * Get disinfectant level status
   *
   * @return disinfectant level status
   */
  liquid::status disinfectant_level() const;

 private:
  /**
   * Liquid Refilling constructor
   *
   * @param water_level_device_id        water level device id
   * @param disinfectant_level_device_id disinfectant level device id
   */
  LiquidRefillingImpl(const std::string& water_level_device_id,
                      const std::string& disinfectant_level_device_id);
  /**
   * Liquid Refilling destructor
   */
  ~LiquidRefillingImpl();
  /**
   * Setup devices
   */
  void setup_devices();
  /**
   * Get water level device id
   *
   * @return water level device id
   */
  inline const std::string& water_level_device_id() const {
    return water_level_device_id_;
  }
  /**
   * Get disinfectant level device id
   *
   * @return disinfectant level device id
   */
  inline const std::string& disinfectant_level_device_id() const {
    return disinfectant_level_device_id_;
  }
  /**
   * Get water level device id
   *
   * @return water level device id
   */
  inline const std::shared_ptr<device::UltrasonicDevice>& water_level_device()
      const {
    return water_level_device_;
  }
  /**
   * Get disinfectant level device id
   *
   * @return disinfectant level device id
   */
  inline const std::shared_ptr<device::UltrasonicDevice>&
  disinfectant_level_device() const {
    return disinfectant_level_device_;
  }

 private:
  /**
   * Active status
   */
  bool active_;
  /**
   * Water level device id
   */
  std::string water_level_device_id_;
  /**
   * Disinfectant level device id
   */
  std::string disinfectant_level_device_id_;
  /**
   * Water level device
   */
  std::shared_ptr<device::UltrasonicDevice> water_level_device_;
  /**
   * Disinfectant level device
   */
  std::shared_ptr<device::UltrasonicDevice> disinfectant_level_device_;
};
}  // namespace impl
}  // namespace mechanism

NAMESPACE_END

#endif  // LIB_MECHANISM_MOVEMENT_HPP_
